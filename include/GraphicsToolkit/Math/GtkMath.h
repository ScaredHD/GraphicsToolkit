#pragma once

#include <cmath>
#include <limits>
#include <functional>

namespace gtk
{
constexpr double inf = std::numeric_limits<double>::infinity();
constexpr double pi = 3.14159265359;
constexpr double piDiv2 = 1.57079632679;
constexpr double piDiv4 = 0.78539816339;
constexpr double invPi = 0.31830988618;
constexpr double inv2Pi = 0.15915494309;
constexpr double inv4Pi = 0.07957747154;
}  // namespace gtk

double ToRad(double deg);

double ToDeg(double rad);

template<typename T>
T Clamp(T val, T lo, T hi)
{
  return (val > lo ? (val < hi ? val : hi) : lo);
}

template<typename T>
T Saturate(T val)
{
  return Clamp(val, T{0}, T{1});
}

template<typename T>
T Wrap(T val, T lo, T hi)
{
  if (hi < lo) {
    return Wrap(val, hi, lo);
  }
  return (val > 0 ? lo : hi) + std::fmod(val, hi - lo);
}

template<typename T>
T Remap(T x, T l0, T r0, T l1, T r1)
{
  return l1 + ((x - l0) * (r1 - l1) / (r0 - l0));
}

template<typename T>
T RemapClamped(T x, T l0, T r0, T l1, T r1)
{
  x = Clamp(x, l0, r0);
  Remap(x, l0, r0, l1, r1);
}

template<typename T>
T Lerp(T min, T max, T t)
{
  return min + t * (max - min);
}

template<typename T>
T Smoothstep(T l, T r, T x)
{
  x = Clamp((x - l) / (r - l));
  return x * x * (T{3} - T{2} * x);
}

template<typename T>
T Linearstep(T l, T r, T x)
{
  return Clamp((x - l) / (r - l));
}

template<typename T>
T Smootherstep(T l, T r, T x)
{
  x = Clamp((x - l) / (r - l));
  return x * x * x * (x * (x * T{6} - T{15}) + T{10});
}