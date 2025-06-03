#include "sample.h"

#include <chrono>
#include <random>

#include <fmt/core.h>

#include "gtk_math.h"

namespace {

auto s_gen = std::mt19937{static_cast<std::mt19937::result_type>(
    std::chrono::high_resolution_clock::now().time_since_epoch().count())};

std::uniform_real_distribution<double> s_dist(0., 1.);

}  // namespace

using namespace gtk;

ProbabilityDensityFunction1D::ProbabilityDensityFunction1D(double domainMin, double domainMax,
                                                       double argMax)
    : domainMin{domainMin},
      domainMax{domainMax},
      argMax{argMax} {

  if (domainMax == inf || domainMin == inf || domainMax == -inf || domainMin == -inf)
    throw std::runtime_error{fmt::format("Invalid PDF domain: domain must be finite")};

  if (domainMin > argMax || argMax > domainMax)
    throw std::runtime_error{
        fmt::format("Invalid PDF domain (min, max, argmax) = ({}, {}, {}): domain can not be empty "
                    "and should contain argmax",
                    domainMin, domainMax, argMax)};
}

double SampleUniform1D(double lo, double hi) {
  double x = s_dist(s_gen);
  return Remap(x, 0., 1., lo, hi);
}

std::tuple<double, double> SampleUniform2D(double lo, double hi) {
  double x = SampleUniform1D(lo, hi);
  double y = SampleUniform1D(lo, hi);
  return {x, y};
}

std::tuple<double, double, double> SampleUniform3D(double lo, double hi) {
  double x = SampleUniform1D(lo, hi);
  double y = SampleUniform1D(lo, hi);
  double z = SampleUniform1D(lo, hi);
  return {x, y, z};
}

double Sample1D(const ProbabilityDensityFunction1D& pdf) {
  return RejectionSample1D(pdf.domainMin, pdf.domainMax, std::ref(pdf), pdf.UpperBound());
}

double RejectionSample1D(double lo, double hi, const std::function<double(double)>& pdf,
                         double upperBound) {
  while (true) {
    double x = SampleUniform1D(lo, hi);
    double u = SampleUniform1D();
    if (u * upperBound < pdf(x)) {
      return x;
    }
  }
}

double NormalDistribution::operator()(double x) const {
  using gtk::pi;
  double c = 1.0 / (sigma * std::sqrt(2 * pi));
  double k = (x - mu) / sigma;
  double n = -0.5 * (k * k);
  return c * std::exp(n);
}

NormalDistribution::NormalDistribution(double mu, double sigma)
    : ProbabilityDensityFunction1D(mu - 3 * sigma, mu + 3 * sigma, mu),
      mu{mu},
      sigma{sigma} {}
