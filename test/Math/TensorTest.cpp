
#include "Tensor.h"

#include <gtest/gtest.h>

#include "Dimension.h"
#include "TensorOperations.h"


template<typename T1, typename T2>
bool AreNearlyEqual(const T1& a, const T2& b, float epsilon = 1e-5f)
{
  if (T1::DimensionType::count != T2::DimensionType::count) {
    return false;
  }
  for (size_t i = 0; i < T1::DimensionType::count; ++i) {
    if (std::abs(a[i] - b[i]) > epsilon) {
      return false;
    }
  }
  return true;
}


TEST(Math, Tensor)
{
  // Default constructor (scalar)
  {
    Tensor<float> s0 = 1.f;
    Tensor<float> s1 = 2.f;
    Tensor<float> s2 = 1.f;
    EXPECT_FALSE(AreNearlyEqual(s0, s1));
    EXPECT_TRUE(AreNearlyEqual(s0, s2));
  }

  // Variadic constructor with single values
  {
    Tensor<int, 3> v{1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
  }

  // Variadic constructor for matrix
  {
    Tensor<float, 3, 2> m0{1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
    EXPECT_EQ(m0(0, 0), 1.f);
    EXPECT_EQ(m0(0, 1), 2.f);
    EXPECT_EQ(m0(1, 0), 3.f);
    EXPECT_EQ(m0(1, 1), 4.f);
    EXPECT_EQ(m0(2, 0), 5.f);
    EXPECT_EQ(m0(2, 1), 6.f);
  }

  // Initializer list constructor
  {
    Tensor<double, 3, 2> m1{{1, 2}, {3, 4}, {5, 6}};
    EXPECT_EQ(m1(0, 0), 1.0);
    EXPECT_EQ(m1(0, 1), 2.0);
    EXPECT_EQ(m1(1, 0), 3.0);
    EXPECT_EQ(m1(1, 1), 4.0);
    EXPECT_EQ(m1(2, 0), 5.0);
    EXPECT_EQ(m1(2, 1), 6.0);
  }

  // Comparison between different constructors
  {
    Tensor<float, 3, 2> m0{1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
    Tensor<double, 3, 2> m1{{1, 2}, {3, 4}, {5, 6}};
    EXPECT_TRUE(AreNearlyEqual(m0, m1));
  }

  // Deduction guide with initializer list
  {
    Tensor<int, 2, 3> t0 = Tensor<int, 2, 3>{{1, 2, 3}, {4, 5, 6}};
    Tensor<int, 2, 3> t1{1, 2, 3, 4, 5, 6};
    EXPECT_TRUE(t0 == t1);
  }

  // 3D tensor constructor
  {
    Tensor<int, 2, 2, 2> t3d{1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(t3d(0, 0, 0), 1);
    EXPECT_EQ(t3d(0, 0, 1), 2);
    EXPECT_EQ(t3d(0, 1, 0), 3);
    EXPECT_EQ(t3d(0, 1, 1), 4);
    EXPECT_EQ(t3d(1, 0, 0), 5);
    EXPECT_EQ(t3d(1, 0, 1), 6);
    EXPECT_EQ(t3d(1, 1, 0), 7);
    EXPECT_EQ(t3d(1, 1, 1), 8);
  }

  // Copy constructor from different type
  {
    Tensor<int, 3> source{1, 2, 3};
    Tensor<float, 3> target(source);
    EXPECT_EQ(target[0], 1.f);
    EXPECT_EQ(target[1], 2.f);
    EXPECT_EQ(target[2], 3.f);
  }

  // Copy constructor with dimension truncation
  {
    Tensor<int, 6> source{1, 2, 3, 4, 5, 6};
    Tensor<int, 3> target(source);
    EXPECT_EQ(target[0], 1);
    EXPECT_EQ(target[1], 2);
    EXPECT_EQ(target[2], 3);
  }

  // Single element tensor
  {
    Tensor<double, 1> scalar{42.0};
    EXPECT_EQ(scalar[0], 42.0);
  }

  // Zero initialization
  {
    Tensor<int, 3> zeros{0, 0, 0};
    EXPECT_EQ(zeros[0], 0);
    EXPECT_EQ(zeros[1], 0);
    EXPECT_EQ(zeros[2], 0);
  }

  // Mixed type constructor compatibility
  {
    Tensor<float, 2, 2> mixed{1, 2.5f, 3.14, 4};
    EXPECT_EQ(mixed(0, 0), 1.f);
    EXPECT_EQ(mixed(0, 1), 2.5f);
    EXPECT_NEAR(mixed(1, 0), 3.14f, 1e-5f);
    EXPECT_EQ(mixed(1, 1), 4.f);
  }


  // Tensor cast
  {
    // Test case 1: (1, 1) -> (3, 3) - matrix broadcasting
    {
      Tensor<int, 1, 1> source{42};
      auto result = Cast<TDimension<3, 3>>(source);

      // All elements should be 42
      for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
          EXPECT_EQ(result(i, j), 42);
        }
      }
    }

    // Test case 2: (1, 3) -> (4, 3) - first dimension broadcasting
    {
      Tensor<int, 1, 3> source{1, 2, 3};
      auto result = Cast<TDimension<4, 3>>(source);

      // Each row should be [1, 2, 3]
      for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(result(i, 0), 1);
        EXPECT_EQ(result(i, 1), 2);
        EXPECT_EQ(result(i, 2), 3);
      }
    }

    // Test case 3: (1, 2, 1) -> (3, 2, 2) - first and third dimension broadcasting
    {
      Tensor<int, 1, 2, 1> source{10, 20};
      auto result = Cast<TDimension<3, 2, 2>>(source);

      // Verify broadcasting pattern
      for (size_t i = 0; i < 3; ++i) {
        for (size_t k = 0; k < 2; ++k) {
          EXPECT_EQ(result(i, 0, k), 10);  // First element broadcast
          EXPECT_EQ(result(i, 1, k), 20);  // Second element broadcast
        }
      }
    }
  }
  {

    // Additional test: (2, 1) -> (2, 4) - second dimension broadcasting
    {
      Tensor<float, 2, 1> source{1.5f, 2.5f};
      auto result = Cast<TDimension<2, 4>>(source);

      // First row should all be 1.5, second row should all be 2.5
      for (size_t j = 0; j < 4; ++j) {
        EXPECT_EQ(result(0, j), 1.5f);
        EXPECT_EQ(result(1, j), 2.5f);
      }
    }

    // Edge case: same dimensions (no broadcasting needed)
    {
      Tensor<int, 2, 3> source{1, 2, 3, 4, 5, 6};
      auto result = Cast<TDimension<2, 3>>(source);

      // Should be identical
      for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
          EXPECT_EQ(result(i, j), source(i, j));
        }
      }
    }

    // Complex broadcasting: (1, 1, 3) -> (2, 4, 3)
    {
      Tensor<double, 1, 1, 3> source{1.0, 2.0, 3.0};
      auto result = Cast<TDimension<2, 4, 3>>(source);

      // All layers should contain the same [1, 2, 3] pattern
      for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 4; ++j) {
          EXPECT_EQ(result(i, j, 0), 1.0);
          EXPECT_EQ(result(i, j, 1), 2.0);
          EXPECT_EQ(result(i, j, 2), 3.0);
        }
      }
    }

    // Implicit cast from Tensor<1> to scalar
    {
      Tensor<int, 1> singleValue{42};
      int scalarValue = singleValue;  // Implicit conversion to scalar
      EXPECT_EQ(scalarValue, 42);
    }
  }
  // Tensor componentwise operations
  {
    // Addition
    {
      Tensor<int, 2, 3> t1{1, 2, 3, 4, 5, 6};
      Tensor<int, 2, 3> t2{10, 20, 30, 40, 50, 60};
      auto result = t1 + t2;

      EXPECT_EQ(result(0, 0), 11);
      EXPECT_EQ(result(0, 1), 22);
      EXPECT_EQ(result(0, 2), 33);
      EXPECT_EQ(result(1, 0), 44);
      EXPECT_EQ(result(1, 1), 55);
      EXPECT_EQ(result(1, 2), 66);
    }

    // Subtraction
    {
      Tensor<float, 3> t1{10.5f, 20.5f, 30.5f};
      Tensor<float, 3> t2{0.5f, 10.5f, 20.5f};
      auto result = t1 - t2;

      EXPECT_EQ(result[0], 10.0f);
      EXPECT_EQ(result[1], 10.0f);
      EXPECT_EQ(result[2], 10.0f);
    }

    // Multiplication
    {
      Tensor<double, 2, 2> t1{2.0, 3.0, 4.0, 5.0};
      Tensor<double, 2, 2> t2{1.5, 2.0, 2.5, 3.0};
      auto result = t1 * t2;

      EXPECT_EQ(result(0, 0), 3.0);
      EXPECT_EQ(result(0, 1), 6.0);
      EXPECT_EQ(result(1, 0), 10.0);
      EXPECT_EQ(result(1, 1), 15.0);
    }

    // Division
    {
      Tensor<float, 3> t1{20.0f, 15.0f, 12.0f};
      Tensor<float, 3> t2{4.0f, 3.0f, 2.0f};
      auto result = t1 / t2;

      EXPECT_EQ(result[0], 5.0f);
      EXPECT_EQ(result[1], 5.0f);
      EXPECT_EQ(result[2], 6.0f);
    }

    // Scalar addition
    {
      Tensor<int, 2, 2> t{1, 2, 3, 4};
      auto result = t + 10;

      EXPECT_EQ(result(0, 0), 11);
      EXPECT_EQ(result(0, 1), 12);
      EXPECT_EQ(result(1, 0), 13);
      EXPECT_EQ(result(1, 1), 14);
    }

    // Scalar multiplication
    {
      Tensor<float, 3> t{1.5f, 2.5f, 3.5f};
      auto result = t * 2.0f;

      EXPECT_EQ(result[0], 3.0f);
      EXPECT_EQ(result[1], 5.0f);
      EXPECT_EQ(result[2], 7.0f);
    }

    // Scalar subtraction
    {
      Tensor<int, 2, 2> t{10, 20, 30, 40};
      auto result = t - 5;

      EXPECT_EQ(result(0, 0), 5);
      EXPECT_EQ(result(0, 1), 15);
      EXPECT_EQ(result(1, 0), 25);
      EXPECT_EQ(result(1, 1), 35);
    }

    // Scalar division
    {
      Tensor<float, 2, 3> t{6.0f, 12.0f, 18.0f, 24.0f, 30.0f, 36.0f};
      auto result = t / 3.0f;

      EXPECT_EQ(result(0, 0), 2.0f);
      EXPECT_EQ(result(0, 1), 4.0f);
      EXPECT_EQ(result(0, 2), 6.0f);
      EXPECT_EQ(result(1, 0), 8.0f);
      EXPECT_EQ(result(1, 1), 10.0f);
      EXPECT_EQ(result(1, 2), 12.0f);
    }
  }

  // Componentwise operations with broadcasting
  {
    // Broadcasting: (1, 3) + (2, 3) -> (2, 3)
    {
      Tensor<int, 1, 3> t1{1, 2, 3};  // Will be broadcast to (2, 3)
      Tensor<int, 2, 3> t2{10, 20, 30, 40, 50, 60};
      auto result = t1 + t2;

      // First row: [1, 2, 3] + [10, 20, 30] = [11, 22, 33]
      EXPECT_EQ(result(0, 0), 11);
      EXPECT_EQ(result(0, 1), 22);
      EXPECT_EQ(result(0, 2), 33);

      // Second row: [1, 2, 3] + [40, 50, 60] = [41, 52, 63]
      EXPECT_EQ(result(1, 0), 41);
      EXPECT_EQ(result(1, 1), 52);
      EXPECT_EQ(result(1, 2), 63);
    }

    // Broadcasting: (3, 1) * (3, 2) -> (3, 2)
    {
      Tensor<float, 3, 1> t1{2.0f, 3.0f, 4.0f};  // Will be broadcast to (3, 2)
      Tensor<float, 3, 2> t2{1.0f, 5.0f, 2.0f, 6.0f, 3.0f, 7.0f};
      auto result = t1 * t2;

      // First row: [2, 2] * [1, 5] = [2, 10]
      EXPECT_EQ(result(0, 0), 2.0f);
      EXPECT_EQ(result(0, 1), 10.0f);

      // Second row: [3, 3] * [2, 6] = [6, 18]
      EXPECT_EQ(result(1, 0), 6.0f);
      EXPECT_EQ(result(1, 1), 18.0f);

      // Third row: [4, 4] * [3, 7] = [12, 28]
      EXPECT_EQ(result(2, 0), 12.0f);
      EXPECT_EQ(result(2, 1), 28.0f);
    }

    // Broadcasting: (1, 1) + (2, 3) -> (2, 3)
    {
      Tensor<int, 1, 1> t1{100};  // Single value broadcast to all positions
      Tensor<int, 2, 3> t2{1, 2, 3, 4, 5, 6};
      auto result = t1 + t2;

      EXPECT_EQ(result(0, 0), 101);
      EXPECT_EQ(result(0, 1), 102);
      EXPECT_EQ(result(0, 2), 103);
      EXPECT_EQ(result(1, 0), 104);
      EXPECT_EQ(result(1, 1), 105);
      EXPECT_EQ(result(1, 2), 106);
    }

    // Broadcasting: (2, 1, 1) - (2, 3, 2) -> (2, 3, 2)
    {
      Tensor<double, 2, 1, 1> t1{10.0, 20.0};  // Broadcast across last two dimensions
      Tensor<double, 2, 3, 2> t2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
      auto result = t1 - t2;

      // First slice (10 - values)
      EXPECT_EQ(result(0, 0, 0), 9.0);  // 10 - 1
      EXPECT_EQ(result(0, 0, 1), 8.0);  // 10 - 2
      EXPECT_EQ(result(0, 1, 0), 7.0);  // 10 - 3
      EXPECT_EQ(result(0, 1, 1), 6.0);  // 10 - 4
      EXPECT_EQ(result(0, 2, 0), 5.0);  // 10 - 5
      EXPECT_EQ(result(0, 2, 1), 4.0);  // 10 - 6

      // Second slice (20 - values)
      EXPECT_EQ(result(1, 0, 0), 13.0);  // 20 - 7
      EXPECT_EQ(result(1, 0, 1), 12.0);  // 20 - 8
      EXPECT_EQ(result(1, 1, 0), 11.0);  // 20 - 9
      EXPECT_EQ(result(1, 1, 1), 10.0);  // 20 - 10
      EXPECT_EQ(result(1, 2, 0), 9.0);   // 20 - 11
      EXPECT_EQ(result(1, 2, 1), 8.0);   // 20 - 12
    }

    // Mixed broadcasting with scalars and tensors
    {
      Tensor<float, 1, 2> t1{0.5f, 1.5f};
      auto scalar_mult = t1 * 4.0f;  // Scalar multiplication
      Tensor<float, 3, 2> t2{2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f};
      auto broadcast_add = scalar_mult + t2;  // Broadcasting addition

      // scalar_mult should be [2.0f, 6.0f] repeated 3 times
      // Adding to t2: [2+2, 6+4], [2+6, 6+8], [2+10, 6+12]
      EXPECT_EQ(broadcast_add(0, 0), 4.0f);   // 2 + 2
      EXPECT_EQ(broadcast_add(0, 1), 10.0f);  // 6 + 4
      EXPECT_EQ(broadcast_add(1, 0), 8.0f);   // 2 + 6
      EXPECT_EQ(broadcast_add(1, 1), 14.0f);  // 6 + 8
      EXPECT_EQ(broadcast_add(2, 0), 12.0f);  // 2 + 10
      EXPECT_EQ(broadcast_add(2, 1), 18.0f);  // 6 + 12
    }
  }
}