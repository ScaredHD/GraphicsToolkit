#include "tensor.h"

#include <gtest/gtest.h>

#include "tensor_operations.h"


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
  {
    Tensor<float> s0 = 1.f;
    Tensor<float> s1 = 2.f;
    Tensor<float> s2 = 1.f;
    EXPECT_FALSE(AreNearlyEqual(s0, s1));
    EXPECT_TRUE(AreNearlyEqual(s0, s2));
  }

  {
    Tensor<float, 3, 2> m0{1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
    Tensor<double, 3, 2> m1{{1, 2}, {3, 4}, {5, 6}};
    EXPECT_TRUE(AreNearlyEqual(m0, m1));
  }

  {
    Tensor<float, 3, 3> m{{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}, {7.f, 8.f, 9.f}};
    Tensor<double> s = 4.f;
    Tensor<float, 3, 3> sum = m + s;



  }

}