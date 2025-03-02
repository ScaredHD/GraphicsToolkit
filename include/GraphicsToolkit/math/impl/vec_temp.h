#ifndef VEC_TEMP_H
#define VEC_TEMP_H

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <numeric>


template<typename Scalar, size_t m, size_t n>
class Mat;

template<typename Scalar, size_t l>
class Vec {
public:
  using ScalarType = Scalar;
  using Iterator = Scalar*;
  using ConstIterator = const Scalar*;

  static constexpr size_t len = l;

  static constexpr Vec Zero() { return {0., 0., 0.}; }

  template<typename... Scalars>
  Vec(Scalars... args) {
    size_t i{};
    auto x = {0, (data_[i++] = args, 0)...};
  }

  Vec(std::initializer_list<Scalar> il) {
    size_t i = 0;
    for (auto e : il)
      data_[i++] = e;
  }

  Vec(const Mat<Scalar, len, 1>& mat) { std::copy(mat.cbegin(), mat.cend(), begin()); }

  Scalar& operator()(size_t i) { return data_[i]; }

  const Scalar& operator()(size_t i) const { return data_[i]; }

  Scalar& operator[](size_t i) { return data_[i]; }

  const Scalar& operator[](size_t i) const { return data_[i]; }

  Iterator begin() { return data_.data(); }

  Iterator end() { return data_.data() + len; }

  ConstIterator cbegin() const { return data_.data(); }

  ConstIterator cend() const { return data_.data() + len; }

  template<typename BinaryOp>
  Vec& ComponentwiseOperation(const Vec& other, BinaryOp f) {
    std::transform(begin(), end(), other.cbegin(), begin(), f);
    return *this;
  }

  Vec operator-() const { return Zero() -= *this; }

  Vec& operator+=(const Vec& other) { return ComponentwiseOperation(other, std::plus<>{}); }

  Vec& operator-=(const Vec& other) { return ComponentwiseOperation(other, std::minus<>{}); }

  Vec& operator*=(const Vec& other) { return ComponentwiseOperation(other, std::multiplies<>{}); }

  Vec& operator/=(const Vec& other) { return ComponentwiseOperation(other, std::divides<>{}); }

  template<typename Fn>
  Vec& ForEachComponent(Fn f) {
    std::for_each(begin(), end(), f);
    return *this;
  }

  Vec& operator*=(Scalar x) {
    return ForEachComponent([x](Scalar& e) { e *= x; });
  }

  Vec& operator/=(Scalar x) {
    return ForEachComponent([x](Scalar& e) { e /= x; });
  }

  Scalar Dot(const Vec& other) const {
    return std::transform_reduce(cbegin(), cend(), other.cbegin(), Scalar{}, std::plus<>{},
                                 std::multiplies<>{});
  }

  Scalar Norm() const { return std::sqrt(Dot(*this)); }

  void Normalize() { *this /= Norm(); }

  Vec Normalized() const {
    auto res = *this;
    return res /= res.Norm();
  }

private:
  std::array<Scalar, len> data_ = {};
};

template<typename Scalar, size_t len>
auto begin(Vec<Scalar, len>& v) {
  return v.begin();
}

template<typename Scalar, size_t len>
auto end(Vec<Scalar, len>& v) {
  return v.end();
}

template<typename Scalar, size_t len>
auto cbegin(const Vec<Scalar, len>& v) {
  return v.cbegin();
}

template<typename Scalar, size_t len>
auto cend(const Vec<Scalar, len>& v) {
  return v.cend();
}


#endif  // VEC_TEMP_H