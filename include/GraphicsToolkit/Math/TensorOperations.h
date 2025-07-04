#pragma once

#include "Tensor.h"

template<typename T1, typename T2, typename BinaryOp>
struct ComponentwiseOpHelper {
  using T1Actual = std::conditional_t<IsTensorClassV<T1>, T1, Tensor<T1, 1>>;
  using T2Actual = std::conditional_t<IsTensorClassV<T2>, T2, Tensor<T2, 1>>;
  using D1Actual = typename T1Actual::DimensionType;
  using D2Actual = typename T2Actual::DimensionType;

  static constexpr bool canBroadcastToT1 = CanBroadcast<D2Actual, D1Actual>::value;
  static constexpr bool canBroadcastToT2 = CanBroadcast<D1Actual, D2Actual>::value;
  static_assert(
    canBroadcastToT1 || canBroadcastToT2,
    "Cannot perform componentwise operation: dimensions are not compatible for broadcasting."
  );

  using D = std::conditional_t<canBroadcastToT1, D1Actual, D2Actual>;

  static constexpr auto Impl(const T1Actual& t1, const T2Actual& t2, BinaryOp&& op)
  {
    auto bt1 = Broadcast<D>(t1);
    auto bt2 = Broadcast<D>(t2);
    using Ret = MakeTensorFromDimensionT<decltype(op(t1(0), t2(0))), D>;
    Ret result;
    for (size_t i = 0; i < D::count; ++i) {
      result[i] = std::forward<BinaryOp>(op)(bt1[i], bt2[i]);
    }
    return result;
  };
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