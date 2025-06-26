#pragma once
#include <array>

#include "Dimension.h"

template<typename Scalar, size_t... dims>
class Tensor
{
public:
  using ScalarType = Scalar;
  using DimensionType = TDimension<dims...>;

  // Constructors

  template<typename... Ts>
  static constexpr std::array<Scalar, DimensionType::count> CTArr(Ts... args)
  {
    std::array<Scalar, DimensionType::count> arr{};
    size_t i{};
    auto _ = {0, (arr[i++] = args, 0)...};
    return arr;
  }

  template<typename... Ts>
  constexpr Tensor(Ts... args) : data{CTArr(args...)}
  {
    // Cannot directly assign here because this constructor is constexpr
    // Must use CTArr helper to initialize 'data' in member initializer
    // size_t i{};
    // auto _ = {0, (data[i++] = args, 0)...};
  }

  constexpr explicit Tensor(std::initializer_list<std::initializer_list<ScalarType>> il)
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
  constexpr operator ScalarType() const
  {
    return data[0];
  }

  // Accessors
  // operator[] for flattened index access
  // operator() for multi-dimensional access

  constexpr ScalarType& operator[](int i) { return data[i]; }
  constexpr const ScalarType& operator[](int i) const { return data[i]; }

  template<typename... Ts>
  constexpr ScalarType& operator()(const Ts&... indices)
  {
    return data[DimensionType::FlattenedIndex(indices...)];
  }

  template<typename... Ts>
  constexpr const ScalarType& operator()(const Ts... indices) const
  {
    return data[DimensionType::FlattenedIndex(indices...)];
  }

  void f(size_t i) {}

  bool IsZeroFree() const
  {
    return std::all_of(data.begin(), data.end(), [](const Scalar& val) { return val != 0; });
  }

  bool OneHot() const
  {
    return std::count(data.begin(), data.end(), 1) == 1 &&
           std::count(data.begin(), data.end(), 0) == DimensionType::count - 1;
  }

  // Iterators for range-based for loops
  constexpr auto begin() { return data.begin(); }
  constexpr auto end() { return data.end(); }
  constexpr auto begin() const { return data.begin(); }
  constexpr auto end() const { return data.end(); }
  constexpr auto cbegin() const { return data.cbegin(); }
  constexpr auto cend() const { return data.cend(); }
  constexpr auto rbegin() { return data.rbegin(); }
  constexpr auto rend() { return data.rend(); }

#if !defined(GTK_TEST)
private:
#endif
  std::array<Scalar, DimensionType::count> data;
};


template<typename T>
inline constexpr size_t Rank = T::DimensionType::rank;

template<typename T>
inline constexpr size_t Count = T::DimensionType::count;

template<typename Scalar, typename Dim>
struct MakeTensorType {
};

template<typename Scalar, size_t... dims>
struct MakeTensorType<Scalar, TDimension<dims...>> {
  using Type = Tensor<Scalar, dims...>;
};

template<typename Scalar, typename Dim>
using MakeTensorTypeT = typename MakeTensorType<Scalar, Dim>::Type;


template<typename FromTensor, typename ToDimension>
auto Cast(const FromTensor& t)
{
  using FromDimension = typename FromTensor::DimensionType;

  static_assert(FromDimension::rank <= ToDimension::rank);
  static_assert(IsCompatibleDimV<FromDimension, ToDimension>);
  static_assert(std::is_same_v<BroadcastDimT<FromDimension, ToDimension>, ToDimension>);

  using ToTensor = MakeTensorTypeT<typename FromTensor::ScalarType, ToDimension>;

}
