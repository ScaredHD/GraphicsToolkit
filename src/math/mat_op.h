#ifndef MAT_OP_H
#define MAT_OP_H

#include "mat_temp.h"
#include "shape_traits.h"

template<typename Scalar, size_t m, size_t n>
bool AreNearlyEqual(const Mat<Scalar, m, n>& a, const Mat<Scalar, m, n>& b, double eps = 1e-6) {
  Mat<Scalar, m, n> diff;
  std::transform(a.cbegin(), a.cend(), b.cbegin(), diff.begin(), std::minus<>{});
  return std::all_of(diff.cbegin(), diff.cend(), [eps](auto d) { return std::abs(d) < eps; });
}

template<typename Scalar, size_t m, size_t n>
auto operator+(const Mat<Scalar, m, n>& a, const Mat<Scalar, m, n>& b) {
  auto res = a;
  return res += b;
}

template<typename Scalar, size_t m, size_t n>
auto operator-(const Mat<Scalar, m, n>& a, const Mat<Scalar, m, n>& b) {
  auto res = a;
  return res -= b;
}

template<typename Scalar, size_t m, size_t n>
auto operator*(const Mat<Scalar, m, n>& a, const Mat<Scalar, m, n>& b) {
  auto res = a;
  return res *= b;
}

template<typename Scalar, size_t m, size_t n>
auto operator/(const Mat<Scalar, m, n>& a, const Mat<Scalar, m, n>& b) {
  auto res = a;
  return res /= b;
}

template<typename Scalar, size_t m, size_t n>
auto operator*(const Mat<Scalar, m, n>& mat, Scalar x) {
  auto res = mat;
  return res *= x;
}

template<typename Scalar, size_t m, size_t n>
auto operator/(const Mat<Scalar, m, n>& mat, Scalar x) {
  auto res = mat;
  return res /= x;
}

template<typename Scalar, size_t m, size_t n>
auto operator*(Scalar x, const Mat<Scalar, m, n>& mat) {
  auto res = mat;
  return res *= x;
}

template<typename Scalar, size_t m, size_t n>
auto operator/(Scalar x, const Mat<Scalar, m, n>& mat) {
  auto res = mat;
  return res /= x;
}

// Matrix multiplication A x B
template<typename Scalar, size_t m, size_t n, size_t p>
auto Mul(const Mat<Scalar, m, n>& a, const Mat<Scalar, n, p>& b) {
  Mat<Scalar, m, p> res{};
  for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < p; ++j)
      for (size_t k = 0; k < n; ++k)
        res(i, j) += a(i, k) * b(k, j);
  return res;
}

// Matrix multiplication M_1 x M_2 x ... x M_k
template<typename Scalar, size_t m, size_t n, size_t p, size_t... rowDims, size_t... colDims>
auto Mul(const Mat<Scalar, m, n>& a, const Mat<Scalar, n, p>& b,
         const Mat<Scalar, rowDims, colDims>&... rest) {
  return Mul(a, Mul(b, rest...));
}

template<typename Scalar, size_t m, size_t n>
Mat<Scalar, n, m> Transposed(const Mat<Scalar, m, n>& mat) {
  return mat.Transposed();
}

template<typename Scalar, size_t m>
Scalar Determinant(const Mat<Scalar, m, m>& mat) {
  return mat.Det();
}


#endif  // MAT_OP_H