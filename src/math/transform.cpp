#include "transform.h"

#include "gtk_utils.h"

Transform::Transform(const Mat4d& matrix) : matrix_{matrix}, invMatrix_{matrix.Inverse()} {}

Vec3d Transform::OnPoint(const Vec3d& p) const
{
  return HNormalized(Mul(matrix_, HPoint(p)));
}

Vec3d Transform::OnVec(const Vec3d& v) const
{
  return Truncate(Mul(matrix_, HVector(v)));
}

Vec3d Transform::OnNormal(const Vec3d& n) const
{
  auto m = matrix_.Transpose().Inverse();  // Fix typo here
  return Truncate(Mul(m, HVector(n)));
}

Transform Transform::Then(const Transform& t) const
{
  return Mul(t.matrix_, matrix_);
}

Transform Translate(const Vec3d& v)
{
  // Empty definition
  return Transform();
}

Transform RotateX(double deg)
{
  auto c = std::cos(ToRad(deg));
  auto s = std::sin(ToRad(deg));
  return Mat4d{1, 0, 0,  0,  //
               0, c, -s, 0,  //
               0, s, c,  0,  //
               0, 0, 0,  1};
}

Transform RotateY(double deg)
{
  // Empty definition
  return Transform();
}

Transform RotateZ(double deg)
{
  // Empty definition
  return Transform();
}

Transform Rotate(const Vec3d& axis, double deg)
{
  // Empty definition
  return Transform();
}

Transform Rotate(const Vec3d& xyzDeg)
{
  // Empty definition
  return Transform();
}

Transform Scale(const Vec3d& v)
{
  // Empty definition
  return Transform();
}