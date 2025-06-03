#pragma once

#include <algorithm>
#include <array>

template<typename Scalar, size_t... dims>
class Tensor;

// Tensor dimension
template<size_t... dims>
struct TDimension {
  static constexpr size_t rank = sizeof...(dims);

  static constexpr std::array<size_t, rank> dimensions = {dims...};

  static constexpr size_t count = (dims * ...);

  constexpr size_t operator[](size_t i) { return dimensions[i]; }

  static constexpr bool isScalar = (rank == 0);
  static constexpr bool isVector = (rank == 1);
  static constexpr bool isMatrix = (rank == 2);
  static constexpr bool isTensor = (rank > 2);

  template<typename T>
  static constexpr size_t FlattenedIndex(T index)
  {
    return index;
  }

  template<typename T, typename... Ts>
  static constexpr size_t FlattenedIndex(T index, Ts... rest)
  {
    constexpr size_t i = rank - sizeof...(Ts) - 1;
    return index * TailProduct<i + 1>() + FlattenedIndex(rest...);
  }

  template<size_t i>
  static constexpr size_t TailProduct()
  {
    if constexpr (i >= rank) {
      return 1;
    } else {  // Crucial to use `else` here to avoid infinite recursion
      return dimensions[i] * TailProduct<i + 1>();
    }
  }

  template<typename Scalar>
  using TensorType = Tensor<Scalar, dims...>;
};

// Scalar dimension is viewed as one dimensional vector with a single element
template<>
struct TDimension<> {
  static constexpr size_t rank = 1;
  static constexpr size_t count = 1;

  static constexpr std::array<size_t, rank> dimensions = {1};

  constexpr size_t operator[](size_t i) { return dimensions[i]; }

  static constexpr bool isScalar = true;
  static constexpr bool isVector = false;
  static constexpr bool isMatrix = false;
  static constexpr bool isTensor = false;

  template<size_t i>
  static constexpr size_t TailProduct()
  {
    return 1;
  }

  template<typename Scalar>
  using TensorType = Tensor<Scalar>;
};


template<size_t n, typename T>
struct Prepend {
  constexpr Prepend()
  {
    arr[0] = n;
    for (size_t i = 0; i < T::size; ++i) {
      arr[i + 1] = T::arr[i];
    }
  }
  static constexpr size_t arr[T::size + 1]{};
};


template<typename D1, typename D2>
struct CompatibleTraits {
  static constexpr bool sameDimension = (D1::rank == D2::rank) && (D1::count == D2::count);
  static constexpr bool isCompatible = sameDimension || D1::isScalar || D2::isScalar;
  using CompatibleDim =
    std::conditional_t<sameDimension, D1, std::conditional_t<D1::isScalar, D2, D1>>;
};
