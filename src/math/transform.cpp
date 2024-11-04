#include "transform.h"

Transform::Transform(const Mat4d& matrix) : matrix_{matrix}, invMatrix_{matrix.Inverse()} {}

Transform Compose(const Transform& t1, const Transform& t2) {
  return Mul(t1.Matrix(), t2.Matrix());
}

Vec3d Transform::ApplyOnPoint(const Vec3d& p) {
  return HNormalized(Mul(matrix_, HPoint(p)));
}

Vec3d Transform::ApplyOnVec(const Vec3d& v) {
  return ToVec3(Mul(matrix_, HVector(v)));
}

Vec3d Transform::ApplyOnNormal(const Vec3d& n) {
  auto m = matrix_.Transposed().Inverse();
  return ToVec3(Mul(m, HVector(n)));
}