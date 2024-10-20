#include "sample.h"

#include <chrono>
#include <random>

#include "gtk_utils.h"

namespace {

auto s_gen = std::mt19937{static_cast<std::mt19937::result_type>(
    std::chrono::high_resolution_clock::now().time_since_epoch().count())};

std::uniform_real_distribution<double> s_dist(0., 1.);

}  // namespace

double SampleUniform1D(double lo, double hi) {
  double x = s_dist(s_gen);
  return MapRange(x, 0., 1., lo, hi);
}

std::tuple<double, double> SampleUniform2D(double lo, double hi) {
  double x = SampleUniform1D(lo, hi);
  double y = SampleUniform1D(lo, hi);
  return {x, y};
}
