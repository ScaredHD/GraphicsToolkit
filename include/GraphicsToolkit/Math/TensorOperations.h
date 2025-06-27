#pragma once

#include "Tensor.h"

template<typename T1, typename T2, typename BinaryOp>
struct ComponentwiseOpHelper {
  using T1Actual = std::conditional_t<IsTensorClassV<T1>, T1, Tensor<T1, 1>>;
  using T2Actual = std::conditional_t<IsTensorClassV<T2>, T2, Tensor<T2, 1>>;
  static constexpr auto Impl(const T1Actual& t1, const T2Actual& t2, BinaryOp&& op)
  {
    using D = BroadcastDimT<typename T1Actual::DimensionType, typename T2Actual::DimensionType>;
    auto bt1 = Cast<D>(t1);
    auto bt2 = Cast<D>(t2);
    using Ret = MakeTensorFromDimensionT<decltype(op(t1(0), t2(0))), D>;
    Ret result;
    for (size_t i = 0; i < D::count; ++i) {
      result[i] = std::forward<BinaryOp>(op)(bt1[i], bt2[i]);
    }
    return result;
  }
};

template<typename T1, typename T2, typename BinaryOp>
constexpr auto ComponentwiseOperation(const T1& t1, const T2& t2, BinaryOp&& op)
{
  return ComponentwiseOpHelper<T1, T2, BinaryOp>::Impl(t1, t2, std::forward<BinaryOp>(op));
}

template<typename T1, typename T2>
constexpr auto operator+(const T1& t1, const T2& t2)
{
  return ComponentwiseOperation(t1, t2, [](auto a, auto b) { return a + b; });
}

template<typename T1, typename T2>
constexpr auto operator-(const T1& t1, const T2& t2)
{
  return ComponentwiseOperation(t1, t2, [](auto a, auto b) { return a - b; });
}

template<typename T1, typename T2>
constexpr auto operator*(const T1& t1, const T2& t2)
{
  return ComponentwiseOperation(t1, t2, [](auto a, auto b) { return a * b; });
}

template<typename T1, typename T2>
constexpr auto operator/(const T1& t1, const T2& t2)
{
  return ComponentwiseOperation(t1, t2, [](auto a, auto b) { return a / b; });
}