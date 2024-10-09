#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>


namespace number {
constexpr double inf = std::numeric_limits<double>::infinity();
constexpr double pi = 3.14159265359;
constexpr double piDiv2 = 1.57079632679;
constexpr double piDiv4 = 0.78539816339;
constexpr double invPi = 0.31830988618;
constexpr double inv2Pi = 0.15915494309;
constexpr double inv4Pi = 0.07957747154;
}  // namespace number

template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
T Clamp(T val, T lo, T hi) {
  return val > lo ? (val < hi ? val : hi) : lo;
}

template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
T Saturate(T val) {
  return Clamp(val, 0.f, 1.f);
}

template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
T Wrap(T val, T lo, T hi) {
  if (hi < lo) {
    return Wrap(val, hi, lo);
  }
  return (val > 0 ? lo : hi) + std::fmod(val, hi - lo);
}

#endif  // UTILS_H