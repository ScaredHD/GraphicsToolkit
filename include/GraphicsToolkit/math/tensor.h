#pragma once
#include <array>

#include "dimension.h"

template<typename Scalar, size_t... dims>
class Tensor
{
public:
  using ScalarType = Scalar;
  using DimensionType = TDimension<dims...>;

  // Constructors

  template<typename... Ts>
  Tensor(Ts... args)
  {
    size_t i{};
    auto _ = {0, (data[i++] = args, 0)...};
  }

  explicit Tensor(std::initializer_list<std::initializer_list<ScalarType>> il)
  {
    size_t i{};
    for (auto l : il) {
      for (auto e : l) {
        data[i++] = e;
      }
    }
  }

  // Conversion from another tensor type
  template<typename OtherScalar, size_t... otherDims>
  Tensor(const Tensor<OtherScalar, otherDims...>& other)
  {
    static_assert(
      DimensionType::count <= TDimension<otherDims...>::count,
      "Must provide a tensor with at least as many elements as the target dimension."
    );

    std::copy(other.data.begin(), other.data.begin() + DimensionType::count, data.begin());
  }

  // Implicit conversion to scalar
  template<typename D = DimensionType, typename = std::enable_if_t<D::isScalar>>
  operator ScalarType() const
  {
    return data[0];
  }

  // Accessors
  // operator[] for flattened index access
  // operator() for multi-dimensional access

  ScalarType& operator[](int i) { return data[i]; }
  const ScalarType& operator[](int i) const { return data[i]; }

  template<typename... Ts>
  ScalarType& operator()(Ts... indices)
  {
    return data[DimensionType::FlattenedIndex(indices...)];
  }

  template<typename... Ts>
  const ScalarType& operator()(Ts... indices) const
  {
    return data[DimensionType::FlattenedIndex(indices...)];
  }

  // Runtime functions
  bool ZeroFree() const
  {
    std::all_of(data.begin(), data.end(), [](const Scalar& val) { return val != 0; });
  }

  bool OneHot() const
  {
    return std::count(data.begin(), data.end(), 1) == 1 &&
           std::count(data.begin(), data.end(), 0) == DimensionType::count - 1;
  }

  // Iterators for range-based for loops
  auto begin() { return data.begin(); }
  auto end() { return data.end(); }
  auto begin() const { return data.begin(); }
  auto end() const { return data.end(); }
  auto cbegin() const { return data.cbegin(); }
  auto cend() const { return data.cend(); }
  auto rbegin() { return data.rbegin(); }
  auto rend() { return data.rend(); }

#if !defined(GTK_TEST)
private:
#endif
  std::array<Scalar, DimensionType::count> data;
};


template<typename T>
constexpr size_t Rank = T::rank;