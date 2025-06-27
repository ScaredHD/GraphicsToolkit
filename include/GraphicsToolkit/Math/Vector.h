#pragma once

#include <numeric>

#include "Tensor.h"


template<typename Scalar, size_t len>
using Vector = Tensor<Scalar, len>;


template<typename Scalar, size_t len>
constexpr auto Dot(const Vector<Scalar, len>& lhs, const Vector<Scalar, len>& rhs)
{
  Vector<Scalar, len> prod = lhs * rhs;
  return std::accumulate(prod.begin(), prod.end(), Scalar{});
}

template<typename Scalar>
constexpr auto Cross(const Vector<Scalar, 3>& lhs, const Vector<Scalar, 3>& rhs)
{
  return Vector<Scalar, 3>{
    lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2],
    lhs[0] * rhs[1] - lhs[1] * rhs[0]
  };
}