#pragma once
#include <array>

#include "Dimension.h"
#include "Tuple.h"

template<typename Scalar, size_t... dims>
class Tensor
{
public:
  using ScalarType = Scalar;
  using DimensionType = TDimension<dims...>;
  static constexpr size_t rank = DimensionType::rank;
  static constexpr size_t count = DimensionType::count;

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
  constexpr Tensor(const Tensor<OtherScalar, otherDims...>& other)
  {
    static_assert(
      DimensionType::count <= TDimension<otherDims...>::count,
      "Must provide a tensor with at least as many elements as the target dimension."
    );

    std::copy(other.data.begin(), other.data.begin() + DimensionType::count, data.begin());
  }

  // Implicit conversion to scalar
  template<typename D = DimensionType, typename = std::enable_if_t<D::count == 1>>
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

template<typename Scalar>
class Tensor<Scalar> : public Tensor<Scalar, 1>
{
public:
  Tensor(Scalar x) : Tensor<Scalar, 1>(x) {}
};

template<typename T>
struct IsTensorClass {
  static constexpr bool value = false;
};

template<typename Scalar, size_t... dims>
struct IsTensorClass<Tensor<Scalar, dims...>> {
  static constexpr bool value = true;
};

template<typename T>
static constexpr bool IsTensorClassV = IsTensorClass<T>::value;


template<typename Scalar, size_t... dims, typename OtherScalar, size_t... otherDims>
constexpr bool
operator==(const Tensor<Scalar, dims...>& lhs, const Tensor<OtherScalar, otherDims...>& rhs)
{
  using Dl = typename Tensor<Scalar, dims...>::DimensionType;
  using Dr = typename Tensor<OtherScalar, otherDims...>::DimensionType;
  if (Dl::count != Dr::count || Dl::rank != Dr::rank ||
      std::is_same_v<Scalar, OtherScalar> == false) {
    return false;
  }

  for (size_t i = 0; i < Dl::count; ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }

  return true;
}

template<typename S1, size_t... dims1, typename S2, size_t... dims2>
constexpr bool operator!=(const Tensor<S1, dims1...>& lhs, const Tensor<S2, dims2...>& rhs)
{
  return !(lhs == rhs);
}


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
using MakeTensorFromDimensionT = typename MakeTensorType<Scalar, Dim>::Type;


template<typename D>
struct DimensionAsTuple;

template<size_t... dims>
struct DimensionAsTuple<TDimension<dims...>> {
  static constexpr gtk::Tuple<decltype(dims)...> value = gtk::MakeTuple(dims...);
};

template<typename D>
inline constexpr auto DimensionAsTupleV = DimensionAsTuple<D>::value;


template<typename ToDimension, typename FromTensor>
constexpr auto Broadcast(const FromTensor& t)
{
  using FromDimension = typename FromTensor::DimensionType;

  static_assert(CanBroadcast<FromDimension, ToDimension>::value);

  using FromDimensionPadded = typename PadDim<FromDimension, ToDimension>::D1Padded;

  using ToTensor = MakeTensorFromDimensionT<typename FromTensor::ScalarType, ToDimension>;
  ToTensor result;

  for (size_t i = 0; i < ToDimension::count; ++i) {
    auto toMultiIndexTuple = ToDimension::UnflattenedIndex(i);
    auto fromDimTuple = DimensionAsTupleV<FromDimensionPadded>;
    auto clampedMultiIndexTuple = gtk::Transform(
      toMultiIndexTuple, fromDimTuple,
      [](size_t index, size_t dim) { return std::min(index, dim - 1); }
    );
    auto fromIndex = FromDimension::FlattenedIndex(clampedMultiIndexTuple);
    result[i] = t[fromIndex];
  }
  return result;
}
