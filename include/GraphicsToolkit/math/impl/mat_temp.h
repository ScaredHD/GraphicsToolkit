#ifndef MAT_TEMP_H
#define MAT_TEMP_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>

// Forward declaration
template<typename Scalar, size_t len>
class Vec;

template<typename Scalar, size_t m, size_t n>
class Mat {
public:
  using ScalarType = Scalar;
  using Iterator = Scalar*;
  using ConstIterator = const Scalar*;

  static constexpr size_t RowCount() { return m; }

  static constexpr size_t ColCount() { return n; }

  static constexpr bool isRow = (m == 1);
  static constexpr bool isCol = (n == 1);
  static constexpr bool isScalar = (isRow && isCol);
  static constexpr bool isSquare = (m == n);
  static constexpr size_t rowCount = m;
  static constexpr size_t colCount = n;

  // Static functions

  static auto Zero() { return Mat{}; }

  template<bool b = isSquare, typename = std::enable_if_t<b>>
  static auto Identity() {
    Mat res{};
    for (size_t i = 0; i < m; ++i)
      res(i, i) = Scalar{1};
    return res;
  }

  static auto All(Scalar x) {
    Mat res;
    std::fill(res.begin(), res.end(), x);
    return res;
  }

  // Constructors

  template<typename... Ts>
  Mat(Ts... args) {
    size_t i{};
    auto a = {0, (data_[i++] = args, 0)...};  // First 0 makes this work with empty args.
  }

  explicit Mat(std::initializer_list<std::initializer_list<Scalar>> il) {
    size_t i{};
    for (auto l : il)
      for (auto e : l)
        data_[i++] = e;
  }

  // Conversion from vector
  template<bool isVec = isCol || isRow, typename = std::enable_if_t<isVec>>
  Mat(const Vec<Scalar, m>& v) {
    std::copy(v.cbegin(), v.cend(), begin());
  }

  // Iterators
  Iterator begin() { return &data_[0]; }

  Iterator end() { return &data_[m * n]; }

  ConstIterator cbegin() const { return &data_[0]; }

  ConstIterator cend() const { return &data_[m * n]; }

  Scalar& operator()(size_t i, size_t j) { return data_[i * n + j]; }

  const Scalar& operator()(size_t i, size_t j) const { return data_[i * n + j]; }

  Scalar& operator[](size_t i) { return data_[i]; }

  const Scalar& operator[](size_t i) const { return data_[i]; }

  auto Row(size_t r) const {
    Mat<Scalar, 1, n> res;
    Iterator start = begin() + r * n;
    std::copy(start, start + n, res.begin());
    return res;
  }

  auto Col(size_t c) const {
    Mat<Scalar, m, 1> res;
    for (int i = 0; i < m; ++i)
      res(i, c) = (*this)(i, c);
    return res;
  }

  template<typename BinaryOp>
  Mat& ComponentwiseOperation(const Mat& other, BinaryOp f) {
    std::transform(begin(), end(), other.cbegin(), begin(), f);
    return *this;
  }

  Mat& operator+=(const Mat& other) { return ComponentwiseOperation(other, std::plus<>()); }

  Mat& operator-=(const Mat& other) { return ComponentwiseOperation(other, std::minus<>()); }

  Mat& operator*=(const Mat& other) { return ComponentwiseOperation(other, std::multiplies<>()); }

  Mat& operator/=(const Mat& other) { return ComponentwiseOperation(other, std::divides<>()); }

  template<typename Fn>
  Mat& ForEachComponent(Fn f) {
    std::for_each(begin(), end(), f);
    return *this;
  }

  Mat& operator*=(Scalar x) {
    return ForEachComponent([x](Scalar& e) { e *= x; });
  }

  Mat& operator/=(Scalar x) {
    return ForEachComponent([x](Scalar& e) { e /= x; });
  }

  auto Transposed() const {
    Mat<Scalar, n, m> res;
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        res(i, j) = (*this)(j, i);
    return res;
  }

  auto Minor(size_t i, size_t j) const {
    Mat<Scalar, m - 1, n - 1> res;
    size_t r = 0;
    size_t c = 0;
    Iterator it = res.begin();
    for (size_t r = 0; r < m; ++r) {
      for (size_t c = 0; c < n; ++c) {
        if (r == i || c == j)
          continue;
        *it++ = (*this)(r, c);
      }
    }
    return res;
  }

  template<bool b = isSquare, typename = std::enable_if_t<b>>
  Scalar Det() const {
    if constexpr (isScalar) {
      return data_[0];
    } else {

      auto res = Scalar{};
      for (size_t j = 0; j < n; ++j) {
        auto sign = j % 2 ? Scalar{-1} : Scalar{1};
        res += sign * (*this)(0, j) * Minor(0, j).Det();
      }
      return res;
    }
  }

  template<bool b = isSquare, typename = std::enable_if_t<b>>
  Mat Inverse() const {
    // TODO:
    return {};
  }

private:
  std::array<Scalar, m * n> data_ = {};
};


#endif  // MAT_TEMP_H