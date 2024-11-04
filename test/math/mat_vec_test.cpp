
#include <gtest/gtest.h>

#include "mat.h"
#include "vec.h"

TEST(Math, Mat) {
  {
    Mat2f a{1.f, 2.f, 3.14f, 4.2f};
    Mat2f b{1.f, 2.f, 3.14f, 4.2f};
    EXPECT_TRUE(AreNearlyEqual(a, b));
  }
  {
    Mat2f a = {1.f, 0.f,  //
               0.f, 1.f};
    Mat2f b = Mat2f::Identity();
    EXPECT_TRUE(AreNearlyEqual(a, b));
  }
  {
    Mat2d a{1.0, 2.0, 3.0, 4.0};
    Mat2d b = {1.0, 2.0, 3.0, 4.0};
    Mat2d c = Mat2d{{1.0, 2.0, 3.0, 4.0}};
    Mat2d d = Mat2d{{1.0, 2.0}, {3.0, 4.0}};
    Mat2d e = Mat2d(1, 2, 3, 4);
    Mat2d f{{1, 2}, {3, 4}};
    Mat2d g(1, 2, 3, 4);
    Mat2d h({1, 2, 3, 4});
    Mat2d i({{1, 2}, {3, 4}});

    EXPECT_TRUE(AreNearlyEqual(a, b));
    EXPECT_TRUE(AreNearlyEqual(a, c));
    EXPECT_TRUE(AreNearlyEqual(a, d));
    EXPECT_TRUE(AreNearlyEqual(a, e));
    EXPECT_TRUE(AreNearlyEqual(a, f));
    EXPECT_TRUE(AreNearlyEqual(a, g));
    EXPECT_TRUE(AreNearlyEqual(a, h));
    EXPECT_TRUE(AreNearlyEqual(a, i));
  }
  {
    Mat3f a{{2.f, 2.f, 2.f}, {2.f, 2.f, 2.f}, {2.f, 2.f, 2.f}};
    Mat3f b = Mat3f::All(2.f);
    EXPECT_TRUE(AreNearlyEqual(a, b));
  }
  {
    Mat2d a = {1, 2, 3, 4};
    Mat2d b = {5, 6, 7, 8};
    Mat2d res = a + b;
    Mat2d expected = {6, 8, 10, 12};
    EXPECT_TRUE(AreNearlyEqual(res, expected));
  }
  {
    Mat2d a = {1, 2, 3, 4};
    Mat2d b = {5, 6, 7, 8};
    Mat2d res = a - b;
    Mat2d expected = {-4, -4, -4, -4};
    EXPECT_TRUE(AreNearlyEqual(res, expected));
  }
  {
    Mat2d a = {1, 2, 3, 4};
    Mat2d b = {5, 6, 7, 8};
    Mat2d res = a * b;
    Mat2d expected = {5, 12, 21, 32};
    EXPECT_TRUE(AreNearlyEqual(res, expected));
  }
  {
    Mat2d a = {1, 2, 3, 4};
    Mat2d b = {5, 6, 7, 8};
    Mat2d res = a / b;
    Mat2d expected = {0.2, 1 / 3.0, 3.0 / 7, 0.5};
    EXPECT_TRUE(AreNearlyEqual(res, expected));
  }
  {
    Mat<double, 2, 3> a = {
        1, 2, 3,  //
        4, 5, 6   //
    };
    Mat<double, 3, 4> b = {1, 2,  3,  4,  //
                           5, 6,  7,  8,  //
                           9, 10, 11, 12};
    Mat<double, 2, 4> ab0 = {38, 44, 50, 56, 83, 98, 113, 128};

    auto ab1 = Mul(a, b);
    EXPECT_TRUE(AreNearlyEqual(ab0, ab1));

    Mat<double, 4, 2> c = {2,  3,   //
                           -4, -5,  //
                           -6, -7,  //
                           -8, -9};

    Mat<double, 2, 2> abc0 = {-848, -960, -1928, -2184};
    auto abc1 = Mul(a, b, c);
    auto abc2 = Mul(Mul(a, b), c);
    auto abc3 = Mul(a, Mul(b, c));

    EXPECT_TRUE(AreNearlyEqual(abc0, abc1));
    EXPECT_TRUE(AreNearlyEqual(abc0, abc2));
    EXPECT_TRUE(AreNearlyEqual(abc0, abc3));
  }
  {
    Mat<float, 3, 1> a = Vec3f{1, 2, 3};
    Mat<float, 3, 1> b = {1, 2, 3};
    EXPECT_TRUE(AreNearlyEqual(a, b));
  }
  {
    Mat3f a = {1, 2, 3, 0, 5, 6, -7, -8, -9};
    Mat3f b = {7, 3, 5, 5, 6, 1, 8, -1, -2};

    Mat3f s1 = a + b;
    Mat3f s0 = {8, 5, 8, 5, 11, 7, 1, -9, -11};
    EXPECT_TRUE(AreNearlyEqual(s0, s1));

    Mat3f d1 = a - b;
    Mat3f d0 = {-6, -1, -2, -5, -1, 5, -15, -7, -7};
    EXPECT_TRUE(AreNearlyEqual(d0, d1));

    Mat3f m1 = a * b;
    Mat3f m0 = {7, 6, 15, 0, 30, 6, -56, 8, 18};
    EXPECT_TRUE(AreNearlyEqual(m0, m1));

    Mat3f q1 = a / b;
    Mat3f q0 = {1.0 / 7, 2.0 / 3, 3.0 / 5, 0, 5.0 / 6, 6, -7.0 / 8, 8, 4.5};
    EXPECT_TRUE(AreNearlyEqual(q0, q1));
  }
  {
    Mat3f a = {1, 2, 3, 0, 5, 6, -7, -8, -9};
    float x = 0.5f;
    Mat3f b = x * a;
    Mat3f c = a * x;
    Mat3f d = {0.5, 1, 1.5, 0, 2.5, 3, -3.5, -4, -4.5};
    EXPECT_TRUE(AreNearlyEqual(b, c));
    EXPECT_TRUE(AreNearlyEqual(c, d));
  }
  {
    Mat2f a = {1, 2, 3, 4};
    float d = a.Det();
    EXPECT_NEAR(d, -2.0, 1e-6);

    Mat2f at = a.Transposed();
    Mat2f at0 = {1, 3, 2, 4};
    EXPECT_TRUE(AreNearlyEqual(at, at0));
  }
}

TEST(Math, Vec) {
  {
    Vec3f a{1.2f, 3.4f, 4.2f};
    Vec3f b = {1.2f, 3.4f, 4.2f};
    Vec3f c(1.2f, 3.4f, 4.2f);
    Vec3f d({1.2f, 3.4f, 4.2f});
    Vec3f e = d;
    Vec3f f{e};
    Vec3f g = {e};

    EXPECT_TRUE(AreNearlyEqual(a, b));
    EXPECT_TRUE(AreNearlyEqual(a, c));
    EXPECT_TRUE(AreNearlyEqual(a, d));
    EXPECT_TRUE(AreNearlyEqual(a, e));
    EXPECT_TRUE(AreNearlyEqual(a, f));
    EXPECT_TRUE(AreNearlyEqual(a, g));
  }
  {
    Vec3f a = Mat<float, 3, 1>{1, 2, 3};
    Vec3f b = {1, 2, 3};
    EXPECT_TRUE(AreNearlyEqual(a, b));
  }
  {
    Vec3d a = {1, -2, 3.14};
    Vec3d b = {2.5, -2.5, 1.2};
    double ab = Dot(a, b);
    EXPECT_NEAR(ab, 11.268, 1e-6);
  }
  {
    Mat<float, 3, 1> a = {1, -2, 3.14};
    Vec3f b = {2.5, -2.5, 1.2};
    float ab = Dot(a, b);
    EXPECT_NEAR(ab, 11.268, 1e-6);
  }
  {
    Vec3d a = {1., 2., 3.14};
    Vec3d b = {1.14, 5.14, 19.19};

    Vec3d s1 = a + b;
    Vec3d s0 = {2.14, 7.14, 22.33};
    EXPECT_TRUE(AreNearlyEqual(s0, s1));

    Vec3d d1 = a - b;
    Vec3d d0 = {-0.14, -3.14, -16.05};
    EXPECT_TRUE(AreNearlyEqual(d0, d1));

    Vec3d m1 = a * b;
    Vec3d m0 = {1.14, 10.28, 60.2566};
    EXPECT_TRUE(AreNearlyEqual(m0, m1));

    Vec3d q0 = {1. / 1.14, 2. / 5.14, 3.14 / 19.19};
    Vec3d q1 = a / b;
    EXPECT_TRUE(AreNearlyEqual(q0, q1));
  }
  {
    Vec3d a = {1, 2, 3};
    Vec3d b = {1, 1, 4};

    double ip1 = Dot(a, b);
    double ip0 = 15;
    EXPECT_NEAR(ip0, ip1, 1e-6);

    Mat<double, 3, 1> c = {1, 1, 4};
    double ip2 = Dot(a, c);
    EXPECT_NEAR(ip0, ip2, 1e-6);

    double ip3 = Dot(c, c);
    EXPECT_NEAR(ip3, 18, 1e-6);

    double n = Norm(b);
    EXPECT_NEAR(n, 4.24264068712, 1e-6);

    double n2 = Norm(c);
    EXPECT_NEAR(n2, 4.24264068712, 1e-6);
  }
  {
    Vec3f a = {1, 2, 3};
    Vec3f b = a.Normalized();
    float nb = b.Norm();
    EXPECT_NEAR(nb, 1.f, 1e-6);
  }
  {
    Vec3d x = {1, 0, 0};
    Vec3d y = {0, 1, 0};
    Vec3d z = Cross(x, y);
    Vec3d z0 = {0, 0, 1};
    EXPECT_TRUE(AreNearlyEqual(z, z0));

    Mat<double, 3, 1> y1 = {0, 1, 0};
    Vec3d z1 = Cross(x, y1);
    EXPECT_TRUE(AreNearlyEqual(z1, z0));
  }
  {
    Mat<double, 3, 1> a = {1, 2, 3};
    Vec<double, 3> a1;
    a1 = a;

    Vec<double, 3> b = {1, 2, 3};
    Mat<double, 3, 1> b1;
    b1 = b;

    EXPECT_TRUE(AreNearlyEqual(a, b1));
    EXPECT_TRUE(AreNearlyEqual(a1, b));
  }
  {
    Mat<double, 2, 3> m1 = {1, 2, 3, 4, 5, 6};
    Mat<double, 3, 3> m2 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Vec<double, 3> v = {1, 2, 3};
    Mat<double, 3, 1> m3 = {1, 2, 3};

    auto res1 = Mul(m1, m2, m3);
    auto res2 = Mul(m1, m2, v);
    auto res3 = Mul(m1, Mul(m2, m3));
    auto res4 = Mul(Mul(m1, m2), v);

    Mat<double, 2, 1> res = {228, 516};

    EXPECT_TRUE(AreNearlyEqual(res, res1));
    EXPECT_TRUE(AreNearlyEqual(res, res2));
    EXPECT_TRUE(AreNearlyEqual(res, res3));
    EXPECT_TRUE(AreNearlyEqual(res, res4));
  }
}
