#pragma once

#include <Tensor.h>

template<typename Scalar, size_t row, size_t col>
using Matrix = Tensor<Scalar, row, col>;

template<typename S1, typename S2, size_t r1, size_t c1r2, size_t c2>
constexpr auto Mul(const Matrix<S1, r1, c1r2>& lhs, const Matrix<S2, c1r2, c2>& rhs)
{
  using ResultType = Matrix<decltype(S1{} * S2{}), r1, c2>;
  ResultType result{};
  for (size_t i = 0; i < r1; ++i) {
    for (size_t j = 0; j < c2; ++j) {
      for (size_t k = 0; k < c1r2; ++k) {
        result[i][j] += lhs[i][k] * rhs[k][j];
      }
    }
  }
  return result;
}

template<typename M1, typename M2, typename... Ms>
constexpr auto Mul(const M1& m1, const M2& m2, const Ms&... ms)
{
  auto m12 = Mul(m1, m2);
  if constexpr (sizeof...(Ms) == 0) {
    return m12;
  } else {
    return Mul(m12, ms...);
  }
}

template<typename S, size_t r, size_t c>
constexpr auto Transposed(const Matrix<S, r, c>& m)
{
  using ResultType = Matrix<S, c, r>;
  ResultType result{};
  for (size_t i = 0; i < r; ++i) {
    for (size_t j = 0; j < c; ++j) {
      result[j][i] = m[i][j];
    }
  }
  return result;
}

template<typename S>
constexpr S Det(const Matrix<S, 1, 1>& m)
{
  return m[0];
}

template<typename S, size_t r>
constexpr S Det(const Matrix<S, r, r>& m)
{
}