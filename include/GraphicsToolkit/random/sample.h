#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdexcept>
#include <tuple>

struct ProbabilityDensityFunction1D {
  ProbabilityDensityFunction1D(double domainMin, double domainMax, double argMax);
  virtual double operator()(double x) const = 0;

  double UpperBound() const { return (*this)(argMax) + 0.1; }

  double domainMin;
  double domainMax;
  double argMax;
};

double SampleUniform1D(double lo = 0.f, double hi = 1.f);

std::tuple<double, double> SampleUniform2D(double lo = 0.f, double hi = 1.f);

std::tuple<double, double, double> SampleUniform3D(double lo = 0.f, double hi = 1.f);

double SampleNormal1D(double lo = 0.f, double hi = 1.f);

double Sample1D(const ProbabilityDensityFunction1D& pdf);

double RejectionSample1D(double lo, double hi, const ProbabilityDensityFunction1D& pdf,
                         double upperBound);

struct NormalDistribution : public ProbabilityDensityFunction1D {
  NormalDistribution(double mu, double sigma);

  double operator()(double x) const override;

  double mu;
  double sigma;
};


#endif  // SAMPLE_H