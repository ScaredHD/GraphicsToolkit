#ifndef VEC_OP_H
#define VEC_OP_H


#include "shape_traits.h"
#include "vec_temp.h"

template<typename Scalar, size_t len>
bool AreNearlyEqual(const Vec<Scalar, len>& a, const Vec<Scalar, len>& b, double eps = 1e-6) {
  Vec<Scalar, len> diff;
  std::transform(cbegin(a), cend(a), cbegin(b), begin(diff), std::minus<>{});
  return std::all_of(cbegin(diff), cend(diff), [eps](Scalar x) { return std::abs(x) < eps; });
}

template<typename Scalar, size_t len>
auto operator+(const Vec<Scalar, len>& a, const Vec<Scalar, len>& b) {
  auto res = a;
  return res += b;
}

template<typename Scalar, size_t len>
auto operator-(const Vec<Scalar, len>& a, const Vec<Scalar, len>& b) {
  auto res = a;
  return res -= b;
}

template<typename Scalar, size_t len>
auto operator*(const Vec<Scalar, len>& a, const Vec<Scalar, len>& b) {
  auto res = a;
  return res *= b;
}

template<typename Scalar, size_t len>
auto operator/(const Vec<Scalar, len>& a, const Vec<Scalar, len>& b) {
  auto res = a;
  return res /= b;
}

template<typename Scalar, size_t len>
auto operator*(Scalar x, const Vec<Scalar, len>& v) {
  auto res = v;
  return res *= x;
}

template<typename Scalar, size_t len>
auto operator/(Scalar x, const Vec<Scalar, len>& v) {
  auto res = v;
  return res /= x;
}

// Dot product. Allow Mat<S, l, 1> to implicitly convert to Vec<S, l>.
template<
    typename T1, typename T2, typename V1 = AsColVec<T1>, typename V2 = AsColVec<T2>,
    typename = std::enable_if_t<std::is_same_v<typename V1::ScalarType, typename V2::ScalarType>>>
typename V1::ScalarType Dot(const T1& a, const T2& b) {
  V1 u = a;
  V2 v = b;
  return std::transform_reduce(begin(u), end(u), begin(v), typename V1::ScalarType{}, std::plus<>{},
                               std::multiplies<>{});
}

// Cross product. Allow Mat<S, 3, 1> to Vec<S, 3> conversion.
template<
    typename T1, typename T2, typename V1 = AsColVec<T1>, typename V2 = AsColVec<T2>,
    typename = std::enable_if_t<std::is_same_v<typename V1::ScalarType, typename V2::ScalarType> &&
                                V1::len == V2::len && V1::len == 3>>
auto Cross(const T1& a, const T2& b) {
  Vec<typename V1::ScalarType, 3> res;
  V1 u = a;
  V2 v = b;
  res[0] = u[1] * v[2] - u[2] * v[1];
  res[1] = u[2] * v[0] - u[0] * v[2];
  res[2] = u[0] * v[1] - u[1] * v[0];
  return res;
}

template<typename T, typename V = typename ShapeTraits<T>::AsColVec>
typename V::ScalarType Norm(const T& v) {
  return V{v}.Norm();
}

template<typename T, typename V = typename ShapeTraits<T>::AsColVec>
T Normalized(const T& v) {
  return V{v}.Normalized();
}

template<typename Scalar, size_t len>
void Normalize(Vec<Scalar, len> v) {
  v.Normalize();
}

template<typename T, typename = std::enable_if_t<AsColVec<T>::len == 3>>
Vec<typename T::ScalarType, 4> Homogeneous(const T& v, typename T::ScalarType x) {
  return {v[0], v[1], v[2], x};
}

template<typename T, typename = std::enable_if_t<AsColVec<T>::len == 4>>
Vec<typename T::ScalarType, 3> HNormalized(const T& v) {
  return {v[0] / v[3], v[1] / v[3], v[2] / v[3]};
}

template<typename T, typename = std::enable_if_t<AsColVec<T>::len == 4>>
Vec<typename T::ScalarType, 3> ToVec3(const T& v) {
  return {v[0], v[1], v[2]};
}

template<typename T, typename = std::enable_if_t<AsColVec<T>::len == 3>>
Vec<typename T::ScalarType, 4> HVector(const T& v) {
  return Homogeneous(v, 0.);
}

template<typename T, typename = std::enable_if_t<AsColVec<T>::len == 3>>
Vec<typename T::ScalarType, 4> HPoint(const T& v) {
  return Homogeneous(v, 1.);
}

#endif  // VEC_OP_H