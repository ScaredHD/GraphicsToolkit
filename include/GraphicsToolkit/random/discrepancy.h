#ifndef DISCREPANCY_H_
#define DISCREPANCY_H_

#include <vector>

class PointSet2D {
public:
  PointSet2D(std::vector<double> x, std::vector<double> y) : x_{std::move(x)}, y_{std::move(y)} {}

  size_t Count() const { return x_.size(); }

  std::tuple<double, double> Point(size_t index) const { return {x_[index], y_[index]}; }

private:
  std::vector<double> x_;
  std::vector<double> y_;
};

class PointSet3D {
public:
  PointSet3D(std::vector<double> x, std::vector<double> y, std::vector<double> z)
      : x_{std::move(x)},
        y_{std::move(y)},
        z_{std::move(z)} {}

  size_t Count() const { return x_.size(); }

  std::tuple<double, double, double> Point(size_t index) const {
    return {x_[index], y_[index], z_[index]};
  }

private:
  std::vector<double> x_;
  std::vector<double> y_;
  std::vector<double> z_;
};

double Corput(size_t i, unsigned int base);

std::vector<double> CorputSequence(size_t count, unsigned int base);

PointSet2D Halton2DSequence(size_t count);

PointSet3D Halton3DSequence(size_t count);


#endif  // DISCREPANCY_H_