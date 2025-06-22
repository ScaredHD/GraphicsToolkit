#pragma once

#include <algorithm>

#include "../core/Tuple.h"

template<typename Scalar, size_t... dims>
class Tensor;

// Tensor dimension
template<size_t... dims>
struct TDimension : gtk::Tuple<decltype(dims)...> {

  static constexpr size_t rank = sizeof...(dims);

  static constexpr size_t count = (dims * ...);

  constexpr size_t operator[](size_t i) { return gtk::Get<i>(*this); }

  static constexpr bool isScalar = (rank == 0);
  static constexpr bool isVector = (rank == 1);
  static constexpr bool isMatrix = (rank == 2);
  static constexpr bool isTensor = (rank > 2);

};
