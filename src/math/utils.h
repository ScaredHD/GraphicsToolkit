#ifndef UTILS_H
#define UTILS_H

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

double Clamp(double val, double lo, double hi);

double Saturate(double val);

double Wrap(double val, double lo, double hi);

#endif  // UTILS_H