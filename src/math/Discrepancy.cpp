#include "Discrepancy.h"

double Corput(size_t i, unsigned int base) {
  double res = 0.0;
  double b = 1.0 / base;
  while (i) {
    res += static_cast<double>(i % base) * b;
    b /= base;
    i /= base;
  }
  return res;
}

std::vector<double> CorputSequence(size_t count, unsigned int base) {
  std::vector<double> seq;
  seq.reserve(count);
  for (int i = 0; i < count; ++i) {
    seq.push_back(Corput(i + 1, base));
  }
  return seq;
}

PointSet2D Halton2DSequence(size_t count) {
  auto x = CorputSequence(count, 2);
  auto y = CorputSequence(count, 3);
  return {x, y};
}

PointSet3D Halton3DSequence(size_t count) {
  auto x = CorputSequence(count, 2);
  auto y = CorputSequence(count, 3);
  auto z = CorputSequence(count, 5);
  return {x, y, z};
}
