#pragma once

#include <algorithm>
// #include <numeric>

#include "Tuple.h"


template<size_t... dims>
struct TDimension;

namespace
{
template<typename D>
struct ToSeqHelper;

template<size_t... dims>
struct ToSeqHelper<TDimension<dims...>> {
  using Type = gtk::IntegerSequence<size_t, dims...>;
};

template<typename D>
using ToSeq = typename ToSeqHelper<D>::Type;

template<typename S>
struct ToDimHelper;

template<size_t... integers>
struct ToDimHelper<gtk::IntegerSequence<size_t, integers...>> {
  using Type = TDimension<integers...>;
};

template<typename S>
using ToDim = typename ToDimHelper<S>::Type;

template<size_t n>
constexpr size_t InnerProduct(const std::array<size_t, n>& a, const std::array<size_t, n>& b)
{
  size_t sum = 0;
  for (size_t i = 0; i < n; ++i) {
    sum += a[i] * b[i];
  }
  return sum;
}

}  // namespace

template<typename D, size_t i>
inline constexpr size_t DimGet = gtk::IntegerSequenceGetV<ToSeq<D>, i>;

template<size_t... dims>
struct TDimension {

  static constexpr size_t rank = sizeof...(dims);
  static constexpr size_t count = (dims * ...);

  static constexpr bool isScalar = (rank == 0);
  static constexpr bool isVector = (rank == 1);
  static constexpr bool isMatrix = (rank == 2);
  static constexpr bool isTensor = (rank > 2);

  template<typename IndexSeq, typename Dim, size_t currentIndex, size_t tailProduct, size_t result>
  struct FlattenedIndexHelper;

  // Recursive case for the recursion
  template<size_t... indices, size_t... ds, size_t currentIndex, size_t tailProduct, size_t result>
  struct FlattenedIndexHelper<
    gtk::IndexSequence<indices...>,
    TDimension<ds...>,
    currentIndex,
    tailProduct,
    result> {
    using D = TDimension<ds...>;
    using S = gtk::IndexSequence<indices...>;
    static constexpr size_t currentDim = DimGet<D, currentIndex>;
    static constexpr size_t currentIndexValue = gtk::IntegerSequenceGetV<S, currentIndex>;
    static constexpr size_t value = FlattenedIndexHelper<
      S,
      D,
      currentIndex - 1,
      tailProduct * currentDim,
      result + tailProduct * currentIndexValue>::value;
  };

  // Base case for the recursion
  template<size_t i, size_t... indices, size_t... ds, size_t tailProduct, size_t result>
  struct FlattenedIndexHelper<
    gtk::IndexSequence<i, indices...>,
    TDimension<ds...>,
    0,
    tailProduct,
    result> {
    static constexpr size_t value = result + i * tailProduct;
  };


  // Compile-time only
  template<size_t... indices>
  static constexpr size_t FlattenedIndex()
  {
    using S = gtk::IndexSequence<indices...>;
    using D = TDimension<dims...>;
    constexpr auto rank = sizeof...(indices);
    constexpr size_t startIndex = rank - 1;
    constexpr size_t startIndexValue = gtk::IntegerSequenceGetV<S, startIndex>;
    // constexpr size_t initSum = gtk::IntegerSequenceGetV<S, startIndex>;
    return FlattenedIndexHelper<S, D, startIndexValue, 1, 0>::value;
  }

  static constexpr std::array<size_t, rank> TailProducts()
  {
    std::array<size_t, rank> products{};
    constexpr std::array<size_t, rank> dimensions = {dims...};
    size_t product = 1;
    for (size_t i = rank - 1; i < rank; --i) {
      products[i] = product;
      product *= dimensions[i];
    }
    return products;
  }

  // Can be used at runtime
  template<typename... Indices>
  static constexpr size_t FlattenedIndex(Indices... indices)
  {
    std::array<size_t, rank> tailProducts = TailProducts();
    std::array<size_t, rank> index = {static_cast<size_t>(indices)...};

    return InnerProduct(tailProducts, index);
  }

  template<typename... E>
  static constexpr size_t FlattenedIndex(const gtk::Tuple<E...>& index)
  {
    std::array<size_t, rank> tailProducts = TailProducts();
    std::array<size_t, rank> indices;
    for (size_t i = 0; i < rank; ++i) {
      indices[i] = gtk::Get(index, i);
    }
    return InnerProduct(tailProducts, indices);
  }


  static constexpr gtk::Tuple<decltype(dims)...> UnflattenedIndex(size_t flatIndex)
  {
    std::array<size_t, rank> dimensions{dims...};
    gtk::Tuple<decltype(dims)...> index;
    for (size_t i = rank - 1; i > 0; --i) {
      Get(index, i) = flatIndex % dimensions[i];
      flatIndex /= dimensions[i];
    }
    Get(index, 0) = flatIndex % dimensions[0];
    flatIndex /= dimensions[0];
    return index;
  }
};

template<>
struct TDimension<> : TDimension<1> {
};

template<typename D>
inline constexpr size_t DimFront = DimGet<D, 0>;

template<typename D, size_t e>
using DimPushFront = ToDim<gtk::IntegerSequencePushFrontT<ToSeq<D>, size_t, e>>;

template<typename D>
using DimPopFront = ToDim<gtk::IntegerSequencePopFrontT<ToSeq<D>>>;

template<typename D>
using DimReverse = ToDim<gtk::IntegerSequenceReverseT<ToSeq<D>>>;


template<typename S, typename L, size_t padRank>
struct PadHelper {
  using Temp = typename PadHelper<S, L, padRank - 1>::PaddedS;
  using PaddedS = DimPushFront<Temp, 1>;
};

template<typename S, typename L>
struct PadHelper<S, L, 0> {
  using PaddedS = S;
};

// Pad two dimensions to the same rank by adding leading 1s to the smaller dimension
template<typename D1, typename D2>
struct PadDim {
private:
  static constexpr bool isD1Smaller = (D1::rank < D2::rank);
  static constexpr size_t padRank = isD1Smaller ? (D2::rank - D1::rank) : (D1::rank - D2::rank);

public:
  using D1Padded =
    std::conditional_t<isD1Smaller, typename PadHelper<D1, D2, padRank>::PaddedS, D1>;

  using D2Padded =
    std::conditional_t<isD1Smaller, D2, typename PadHelper<D2, D1, padRank>::PaddedS>;
};

template<typename D1, typename D2>
struct BroadcastAlignedDim;

template<>
struct BroadcastAlignedDim<TDimension<>, TDimension<>> {
  using Type = TDimension<>;
};

template<size_t front1, size_t... dims1, size_t front2, size_t... dims2>
struct BroadcastAlignedDim<TDimension<front1, dims1...>, TDimension<front2, dims2...>> {
  using Type = DimPushFront<
    typename BroadcastAlignedDim<TDimension<dims1...>, TDimension<dims2...>>::Type,
    std::max(front1, front2)>;
};

template<typename D1, typename D2>
using BroadcastAlignedDimT = typename BroadcastAlignedDim<D1, D2>::Type;


template<typename D1, typename D2>
struct IsCompatibleDim;

template<>
struct IsCompatibleDim<TDimension<>, TDimension<>> {
  static constexpr bool value = true;
};

template<size_t n, size_t... dims>
struct IsCompatibleDim<TDimension<n, dims...>, TDimension<>> {
  static constexpr bool value = true;
};

// template<size_t n, size_t... dims>
// struct IsCompatibleDim<TDimension<n, dims...>, TDimension<1>> {
//   static constexpr bool value = true;
// };

template<size_t n, size_t... dims>
struct IsCompatibleDim<TDimension<>, TDimension<n, dims...>> {
  static constexpr bool value = true;
};

// template<size_t n, size_t... dims>
// struct IsCompatibleDim<TDimension<1>, TDimension<n, dims...>> {
//   static constexpr bool value = true;
// };


template<size_t front1, size_t... dims1, size_t front2, size_t... dims2>
struct IsCompatibleDim<TDimension<front1, dims1...>, TDimension<front2, dims2...>> {
private:
  static constexpr bool cond1 = (front1 == front2) || (front1 == 1) || (front2 == 1);
  static constexpr bool cond2 = IsCompatibleDim<TDimension<dims1...>, TDimension<dims2...>>::value;

public:
  static constexpr bool value = cond1 && cond2;
};

template<typename D1, typename D2>
inline constexpr bool IsCompatibleDimV = IsCompatibleDim<D1, D2>::value;

template<typename D1, typename D2, bool isCompatible = IsCompatibleDimV<D1, D2>>
struct BroadcastDimension;

template<typename D1, typename D2>
struct BroadcastDimension<D1, D2, true> {
  using PD1 = typename PadDim<D1, D2>::D1Padded;
  using PD2 = typename PadDim<D1, D2>::D2Padded;
  using Type = BroadcastAlignedDimT<PD1, PD2>;
};

template<typename D1, typename D2>
using BroadcastDimT = typename BroadcastDimension<D1, D2>::Type;
