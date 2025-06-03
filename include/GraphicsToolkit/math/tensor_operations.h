#pragma once

#include <functional>

#include "dimension.h"
#include "tensor.h"


template<
  typename T1,
  typename T2,
  typename BinaryOp,
  typename D1 = typename T1::DimensionType,
  typename D2 = typename T2::DimensionType,
  typename S = typename T1::ScalarType,
  typename D = std::enable_if_t<
    CompatibleTraits<D1, D2>::isCompatible,
    typename CompatibleTraits<D1, D2>::CompatibleDim>,
  typename T = typename D::template TensorType<S>>
T ComponentwiseOperation(const T1& a, const T2& b, BinaryOp op)
{
  auto c = BroadcastTensor<D>(a);
  auto d = BroadcastTensor<D>(b);
  T result;
  std::transform(c.begin(), c.end(), d.begin(), result.begin(), op);
  return result;
}

template<typename T1, typename T2>
auto operator+(const T1& a, const T2& b)
{
  return ComponentwiseOperation(a, b, std::plus<>());
}

template<typename T1, typename T2>
auto operator-(const T1& a, const T2& b)
{
  return ComponentwiseOperation(a, b, std::minus<>());
}

template<typename T1, typename T2>
auto operator*(const T1& a, const T2& b)
{
  return ComponentwiseOperation(a, b, std::multiplies<>());
}

template<typename T1, typename T2>
auto operator/(const T1& a, const T2& b)
{
  return ComponentwiseOperation(a, b, std::divides<>());
}