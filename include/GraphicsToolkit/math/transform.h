#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "mat.h"
#include "vec.h"

class Transform {
public:
  Transform() = default;

  Transform(const Mat4d& matrix);

  Transform(const Vec3d& loc, const Vec3d& rot, double scale);

  Mat4d Matrix() const { return matrix_; }

  Mat4d InvMatrix() const { return invMatrix_; }

  Vec3d ApplyOnPoint(const Vec3d& p);

  Vec3d ApplyOnVec(const Vec3d& v);

  Vec3d ApplyOnNormal(const Vec3d& n);

private:
  Mat4d matrix_ = Mat4d::Identity();
  Mat4d invMatrix_ = Mat4d::Identity();
};

Transform Compose(const Transform& t1, const Transform& t2);

template<typename T1, typename T2, typename... Ts>
Transform Compose(const T1& t1, const T2& t2, const Ts&... ts) {
  return Compose(t1, Compose(t2, ts...));
}


#endif  // TRANSFORM_H