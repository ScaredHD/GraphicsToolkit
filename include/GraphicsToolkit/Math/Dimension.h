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
    return FlattenedIndexHelper<S, D, startIndex, 1, 0>::value;
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

// Specialization for scalar dimensions (rank 0)
template<>
struct TDimension<> {
  static constexpr size_t rank = 0;
  static constexpr size_t count = 1;

  static constexpr bool isScalar = true;
  static constexpr bool isVector = false;
  static constexpr bool isMatrix = false;
  static constexpr bool isTensor = false;

  // TailProducts for scalar - return empty array (can't use std::array<size_t, 0>)
  // Instead, we'll make it a no-op since it's not used for scalars
  static constexpr void TailProducts()
  {
    // No tail products for scalar
  }

  // FlattenedIndex for scalar - no indices needed, always returns 0
  static constexpr size_t FlattenedIndex() { return 0; }

  // Runtime version with variadic args (should be empty for scalar)
  template<typename... Indices>
  static constexpr size_t FlattenedIndex(Indices... indices)
  {
    static_assert(sizeof...(indices) == 0, "Scalar dimension expects no indices");
    return 0;
  }

  // Runtime version with tuple (empty tuple for scalar)
  template<typename... E>
  static constexpr size_t FlattenedIndex(const gtk::Tuple<E...>& index)
  {
    static_assert(sizeof...(E) == 0, "Scalar dimension expects empty tuple");
    return 0;
  }

  // UnflattenedIndex for scalar returns empty tuple
  static constexpr gtk::Tuple<> UnflattenedIndex(size_t) { return {}; }
};

template<typename D>
inline constexpr size_t DimFront = DimGet<D, 0>;

template<typename D, size_t e>
using DimPushFront = ToDim<gtk::IntegerSequencePushFrontT<ToSeq<D>, size_t, e>>;

template<typename D>
using DimPopFront = ToDim<gtk::IntegerSequencePopFrontT<ToSeq<D>>>;

template<typename D>
using DimReverse = ToDim<gtk::IntegerSequenceReverseT<ToSeq<D>>>;

template<typename D>
using DimPopBack = ToDim<gtk::IntegerSequencePopBackT<ToSeq<D>>>;

template<typename D, size_t e>
struct DimRemoveElement {
  template<typename T, T x>
  struct NotEquals {
    static constexpr bool value = (x != e);
  };

  using Type = ToDim<gtk::IntegerSequenceFilterT<ToSeq<D>, NotEquals>>;
};


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

template<typename FromDim, typename ToDim>
struct CanBroadcastHelper {
  static constexpr size_t from0 = DimFront<FromDim>;
  static constexpr size_t to0 = DimFront<ToDim>;
  static constexpr bool headCompatible = (from0 == to0) || (from0 == 1);

  using fromRest = DimPopFront<FromDim>;
  using toRest = DimPopFront<ToDim>;
  static constexpr bool restCompatible = CanBroadcastHelper<fromRest, toRest>::value;

  static constexpr bool value = headCompatible && restCompatible;
};

template<>
struct CanBroadcastHelper<TDimension<>, TDimension<>> {
  static constexpr bool value = true;
};

template<typename FromDim>
struct CanBroadcastHelper<FromDim, TDimension<>> {
  static constexpr bool value = false;
};

// Directional broadcasting
template<typename FromDim, typename ToDim, bool validRank = (FromDim::rank <= ToDim::rank)>
struct CanBroadcast {
  static constexpr bool value = false;  // Default: invalid rank means no broadcasting
};

// Specialization for valid rank cases
template<typename FromDim, typename ToDim>
struct CanBroadcast<FromDim, ToDim, true> {
private:
  using FromDimPadded = typename PadDim<FromDim, ToDim>::D1Padded;

public:
  static constexpr bool value = CanBroadcastHelper<FromDimPadded, ToDim>::value;
};

template<typename D, size_t popFrontCount, size_t popBackCount>
struct DimPopFrontAndBack {
  // First pop all front elements, then pop all back elements
  using AfterPopFront = typename DimPopFrontAndBack<D, popFrontCount, 0>::Type;
  using Type = typename DimPopFrontAndBack<AfterPopFront, 0, popBackCount>::Type;
};

template<typename D>
struct DimPopFrontAndBack<D, 0, 0> {
  using Type = D;
};

template<typename D, size_t popFrontCount>
struct DimPopFrontAndBack<D, popFrontCount, 0> {
  using Type = typename DimPopFrontAndBack<DimPopFront<D>, popFrontCount - 1, 0>::Type;
};

template<typename D, size_t popBackCount>
struct DimPopFrontAndBack<D, 0, popBackCount> {
  using Type = typename DimPopFrontAndBack<DimPopBack<D>, 0, popBackCount - 1>::Type;
};

template<typename D, size_t count, size_t r = D::rank, typename = std::enable_if_t<(count <= r)>>
struct DimExtractFront {
  static constexpr size_t popCount = r - count;
  using Type = typename DimPopFrontAndBack<D, 0, popCount>::Type;
};

template<typename D, size_t count, size_t r = D::rank, typename = std::enable_if_t<(count <= r)>>
struct DimExtractBack {
  static constexpr size_t popCount = r - count;
  using Type = typename DimPopFrontAndBack<D, popCount, 0>::Type;
};

template<typename D1, typename D2>
struct DimConcat;

template<size_t... dims1, size_t... dims2>
struct DimConcat<TDimension<dims1...>, TDimension<dims2...>> {
  using Type = TDimension<dims1..., dims2...>;
};

template<typename D, size_t start, size_t len>
struct SubDimHelper {
  static constexpr size_t popFrontCount = start;
  static constexpr size_t popBackCount = D::rank - (start + len);
  using Type = typename DimPopFrontAndBack<D, popFrontCount, popBackCount>::Type;
  using Head = typename DimExtractFront<D, popFrontCount>::Type;
  using Tail = typename DimExtractBack<D, popBackCount>::Type;
  using Complement = typename DimConcat<Head, Tail>::Type;
};

template<>
struct SubDimHelper<TDimension<>, 0, 0> {
  using Type = TDimension<>;
  using Complement = TDimension<>;
};

template<typename D, size_t start>
struct SubDimHelper<D, start, 0> {
  using Type = TDimension<>;
  using Complement = D;
};

template<
  typename D,
  size_t start,
  size_t len,
  bool isEmpty = (D::rank == 0 && start == 0 && len == 0),
  bool validRange = (start < D::rank && start + len <= D::rank),
  typename = std::enable_if_t<isEmpty || validRange>>
struct SubDim {
  using Type = typename SubDimHelper<D, start, len>::Type;
  using Complement = typename SubDimHelper<D, start, len>::Complement;
};

template<typename D1, typename D2, bool valid>
struct ContainsDimHelper {
  using D1Heads = typename DimExtractFront<D1, D2::rank>::Type;
  static constexpr bool headsMatch = std::is_same_v<D1Heads, D2>;
  using D1Tail = DimPopFront<D1>;
  using D1TailTest = ContainsDimHelper<D1Tail, D2, (D1Tail::rank >= D2::rank)>;
  static constexpr bool value = headsMatch || D1TailTest::value;
  static constexpr size_t index = (headsMatch ? 0 : D1TailTest::index + 1);  // +1 for the head
};

template<typename D1, typename D2>
struct ContainsDimHelper<D1, D2, false> {
  static constexpr bool value = false;
  static constexpr size_t index = 0;  // Default index when not contained
};

template<typename D1>
struct ContainsDimHelper<D1, TDimension<>, true> {
  static constexpr bool value = true;  // Any dimension contains empty dimension
  static constexpr size_t index = 0;
};

template<typename D1, typename D2>
struct ContainsDim {
  static constexpr bool value = ContainsDimHelper<D1, D2, (D1::rank >= D2::rank)>::value;
  static constexpr size_t index = ContainsDimHelper<D1, D2, (D1::rank >= D2::rank)>::index;
};

template<typename FromDim, typename ToDim>
struct CanDegenerate {
private:
  using SearchResult = ContainsDim<FromDim, ToDim>;
  static constexpr bool found = SearchResult::value;
  static constexpr size_t foundAtIndex = SearchResult::index;
  static constexpr bool isDowncast = (FromDim::rank > ToDim::rank);
  using Rest = typename SubDim<FromDim, foundAtIndex, ToDim::rank>::Complement;
  static constexpr bool restIsEmpty = (Rest::rank == 0);
  static constexpr bool restAreAllOnes = DimRemoveElement<Rest, 1>::Type::rank == 0;

public:
  static constexpr bool value = isDowncast && found && (restIsEmpty || restAreAllOnes);
};
