#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>

namespace gtk {
constexpr double inf = std::numeric_limits<double>::infinity();
constexpr double pi = 3.14159265359;
constexpr double piDiv2 = 1.57079632679;
constexpr double piDiv4 = 0.78539816339;
constexpr double invPi = 0.31830988618;
constexpr double inv2Pi = 0.15915494309;
constexpr double inv4Pi = 0.07957747154;
}  // namespace gtk

template<typename T>
T Clamp(T val, T lo, T hi) {
  return val > lo ? (val < hi ? val : hi) : lo;
}

template<typename T>
T Saturate(T val) {
  return Clamp(val, 0.f, 1.f);
}

template<typename T>
T Wrap(T val, T lo, T hi) {
  if (hi < lo) {
    return Wrap(val, hi, lo);
  }
  return (val > 0 ? lo : hi) + std::fmod(val, hi - lo);
}

template<typename T>
T MapRange(T x, T l0, T r0, T l1, T r1) {
  return l1 + ((x - l0) * (r1 - l1) / (r0 - l0));
}

template<typename T>
T MapRangeClamped(T x, T l0, T r0, T l1, T r1) {
  x = Clamp(x, l0, r0);
  MapRange(x, l0, r0, l1, r1);
}

#endif  // UTILS_H