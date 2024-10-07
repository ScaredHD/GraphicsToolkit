#include "utils.h"

#include <cmath>

double Clamp(double val, double lo, double hi) {
  return val > lo ? (val < hi ? val : hi) : lo;
}

double Saturate(double val) {
  return Clamp(val, 0.f, 1.f);
}

double Wrap(double val, double lo, double hi) {
  if (hi < lo) {
    return Wrap(val, hi, lo);
  }
  return (val > 0 ? lo : hi) + std::fmod(val, hi - lo);
}