#include "transform.h"

#include <gtest/gtest.h>

#include "common/test_utils.h"

void AssertTransformEq(const Transform& t1, const Transform& t2)
{
  AssertMatEq(t1.Matrix(), t2.Matrix());
  AssertMatEq(t1.InvMatrix(), t2.InvMatrix());
}

TEST(TransformTest, Constructor)
{
  {
    Transform t;
    AssertTransformEq(t, Transform(Mat4d::Identity()));
  }

  {
    Mat4d m = Mat4d::Identity();
    Transform t{m};
    AssertTransformEq(t, Transform(m));
  }
}

TEST(TransformTest, OnPoint)
{
  {
    Transform t;
    Vec3d p{1, 2, 3};
    AssertVecEq(t.OnPoint(p), p);
  }

  {
    Transform t{Mat4d::Identity()};
    Vec3d p{1, 2, 3};
    AssertVecEq(t.OnPoint(p), p);
  }
}

TEST(TransformTest, OnVec)
{
  {
    Transform t;
    Vec3d v{1, 2, 3};
    AssertVecEq(t.OnVec(v), v);
  }

  {
    Transform t{Mat4d::Identity()};
    Vec3d v{1, 2, 3};
    AssertVecEq(t.OnVec(v), v);
  }
}

TEST(TransformTest, OnNormal)
{
  {
    Transform t;
    Vec3d n{1, 2, 3};
    AssertVecEq(t.OnNormal(n), n);
  }

  {
    Transform t{Mat4d::Identity()};
    Vec3d n{1, 2, 3};
    AssertVecEq(t.OnNormal(n), n);
  }
}

TEST(TransformTest, Compose)
{
  {
    Transform t1;
    Transform t2;
    AssertTransformEq(Compose(t1, t2), Transform(Mat4d::Identity()));
  }

  {
    Transform t1{Mat4d::Identity()};
    Transform t2{Mat4d::Identity()};
    Transform t3{Mat4d::Identity()};
    AssertTransformEq(Compose(t1, t2, t3), Transform(Mat4d::Identity()));
  }

  {
    Transform t1 = RotateX(45);
    Transform t2 = Translate(Vec3d{1, 2, 3});
    auto m12 = Compose(t1, t2).Matrix();
    auto c = std::sqrt(2) / 2;
    auto m = Mat4d{1, 0, 0,  1,  //
                   0, c, -c, 2,  //
                   0, c, c,  3,  //
                   0, 0, 0,  1};
    AssertMatEq(m12, m);
  }
}