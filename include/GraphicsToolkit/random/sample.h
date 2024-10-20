#ifndef SAMPLE_H
#define SAMPLE_H

#include <utility>


double SampleUniform1D(double lo = 0.f, double hi = 1.f);

std::tuple<double, double> SampleUniform2D(double lo = 0.f, double hi = 1.f);

std::tuple<double, double, double> SampleUniform3D(double lo = 0.f, double hi = 1.f);

template<typename F>
double Sample1D(F pdf) {}


#endif  // SAMPLE_H