#include <gtest/gtest.h>

#include "mat.h"
#include "vec.h"


template<typename T, size_t N>
void AssertVecEq(const Vec<T, N>& vec1, const Vec<T, N>& vec2)
{
  for (size_t i = 0; i < N; ++i) {
    if constexpr (std::is_floating_point_v<T>) {
      ASSERT_FLOAT_EQ(vec1[i], vec2[i]) << "Vectors differ at index " << i;
    } else {
      ASSERT_EQ(vec1[i], vec2[i]) << "Vectors differ at index " << i;
    }
  }
}

template<typename T, size_t Rows, size_t Cols>
void AssertMatEq(const Mat<T, Rows, Cols>& mat1, const Mat<T, Rows, Cols>& mat2)
{
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Cols; ++j) {
      if constexpr (std::is_floating_point_v<T>) {
        ASSERT_FLOAT_EQ(mat1(i, j), mat2(i, j))
          << "Matrices differ at element (" << i << ", " << j << ")";
      } else {
        ASSERT_EQ(mat1(i, j), mat2(i, j))
          << "Matrices differ at element (" << i << ", " << j << ")";
      }
    }
  }
}