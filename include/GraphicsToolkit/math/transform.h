#pragma once

#include "mat.h"
#include "vec.h"

class Transform
{
public:
  Transform() = default;

  Transform(const Mat4d& matrix);

  Mat4d Matrix() const { return matrix_; }

  Mat4d InvMatrix() const { return invMatrix_; }

  Transform Inverse() const { return Transform{invMatrix_}; }

  Vec3d OnPoint(const Vec3d& p) const;

  Vec3d OnVec(const Vec3d& v) const;

  Vec3d OnNormal(const Vec3d& n) const;

  Transform Then(const Transform& t) const;

private:
  Mat4d matrix_ = Mat4d::Identity();
  Mat4d invMatrix_ = Mat4d::Identity();
};

template<typename T, typename = std::enable_if_t<std::is_same_v<T, Transform>>>
Transform Compose(const T& t)
{
  return t;
}

// Compose multiple transformations in argument order.
template<
  typename T,
  typename... Ts,
  typename = std::enable_if_t<
    std::conjunction_v<std::is_same<T, Transform>, std::is_same<Ts, Transform>...>>>
Transform Compose(const T& t, const Ts&... rest)
{
  return Mul(t.Matrix(), Compose(rest...).Matrix());
}

Transform Translate(const Vec3d& v);

Transform RotateX(double deg);

Transform RotateY(double deg);

Transform RotateZ(double deg);

Transform Rotate(const Vec3d& axis, double deg);

Transform Rotate(const Vec3d& xyzDeg);

Transform Scale(const Vec3d& v);
