#pragma once

namespace
{
template<bool cond, typename Then, typename Else>
struct If;

template<typename Then, typename Else>
struct If<true, Then, Else> {
  using Type = Then;
};

template<typename Then, typename Else>
struct If<false, Then, Else> {
  using Type = Else;
};

template<bool cond, typename Then, typename Else>
using IfT = typename If<cond, Then, Else>::Type;

template<bool, typename T = void>
struct EnableIf {
};

template<typename T>
struct EnableIf<true, T> {
  using Type = T;
};

template<bool cond, typename T = void>
using EnableIfT = typename EnableIf<cond, T>::Type;

}  // namespace

namespace gtk
{


// Minimum basic operations defined for a type to be generic list
// - IsEmptyV<T>
// - Front<T>::Type
// - PushFront<T>::Type
// - PopFront<T>::Type

template<typename T>
struct IsEmpty {
  static constexpr bool value = false;
};

template<typename T>
struct Front;

template<typename T, typename NewElement>
struct PushFront;

template<typename T>
struct PopFront;

// Aliases
template<typename T>
inline constexpr bool IsEmptyV = IsEmpty<T>::value;

template<typename T>
using FrontT = typename Front<T>::Type;

template<typename T, typename NewElement>
using PushFrontT = typename PushFront<T, NewElement>::Type;

template<typename T>
using PopFrontT = typename PopFront<T>::Type;

// Assuming these operation are defined, other operations can be derived from them
// - Size
// - Get
// - PushBack
// - Reverse
// - PopBack
// - Back
// - Transform
// - Filter
// - FoldLeft / FoldRight

template<typename T>
struct Size {
  static constexpr size_t value = IsEmptyV<T> ? 0 : 1 + Size<PopFrontT<T>>::value;
};

template<typename T>
using SizeV = typename Size<T>::value;

// TODO: Add out of bounds checks for Get
template<typename T, size_t index>
struct TypelistGet {
  using Type = typename TypelistGet<PopFrontT<T>, index - 1>::Type;
};

template<typename T>
struct TypelistGet<T, 0> {
  using Type = FrontT<T>;
};

template<typename T, size_t index>
using GetType = typename TypelistGet<T, index>::Type;

// Optimization available, need customization for specific types
template<typename T, typename NewElement, bool isEmpty = IsEmptyV<T>>
struct PushBackHelper;

template<typename T, typename NewElement>
struct PushBackHelper<T, NewElement, true> {
  using Type = PushFrontT<T, NewElement>;
};

template<typename T, typename NewElement>
struct PushBackHelper<T, NewElement, false> {
  using Type = PushFrontT<typename PushBackHelper<PopFrontT<T>, NewElement>::Type, FrontT<T>>;
};

template<typename T, typename NewElement>
struct PushBack : PushBackHelper<T, NewElement> {
};

template<typename T, typename NewElement>
using PushBackT = typename PushBack<T, NewElement>::Type;


template<typename T, bool isEmpty = IsEmptyV<T>>
struct Reverse;

template<typename T>
struct Reverse<T, true> {
  using Type = T;
};

template<typename T>
struct Reverse<T, false> {
  using Type = PushBackT<typename Reverse<PopFrontT<T>>::Type, FrontT<T>>;
};

template<typename T>
using ReverseT = typename Reverse<T>::Type;

template<typename T, bool isEmpty = IsEmptyV<T>>
struct PopBack;

template<typename T>
struct PopBack<T, true> {
  using Type = T;
};

template<typename T>
struct PopBack<T, false> {
  using Type = ReverseT<PopFrontT<ReverseT<T>>>;
};

template<typename T>
using PopBackT = typename PopBack<T>::Type;

template<typename List>
struct Back {
  using Type = FrontT<ReverseT<List>>;
};

template<typename List>
using BackT = typename Back<List>::Type;

template<typename List, template<typename T> class F, bool isEmpty = IsEmptyV<List>>
struct Transform;

template<typename List, template<typename T> class F>
struct Transform<List, F, true> {
  using Type = List;
};

template<typename List, template<typename T> class F>
struct Transform<List, F, false>
    : PushFront<typename Transform<PopFrontT<List>, F>::Type, typename F<FrontT<List>>::Type> {
};

template<typename List, template<typename T> class F>
using TransformT = typename Transform<List, F>::Type;


template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsEmptyV<List>>
struct FoldLeft;

template<typename List, template<typename T, typename U> class F, typename I>
struct FoldLeft<List, F, I, true> {
  using Type = I;
};

template<typename List, template<typename T, typename U> class F, typename I>
struct FoldLeft<List, F, I, false> {
  using Type = typename F<typename FoldLeft<PopBackT<List>, F, I>::Type, BackT<List>>::Type;
};

template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsEmptyV<List>>
using FoldLeftT = typename FoldLeft<List, F, I, isEmpty>::Type;

template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsEmptyV<List>>
struct FoldRight;

template<typename List, template<typename T, typename U> class F, typename I>
struct FoldRight<List, F, I, true> {
  using Type = I;
};

template<typename List, template<typename T, typename U> class F, typename I>
struct FoldRight<List, F, I, false> {
  using Type = typename F<FrontT<List>, typename FoldRight<PopFrontT<List>, F, I>::Type>::Type;
};

template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsEmptyV<List>>
using FoldRightT = typename FoldLeft<List, F, I, isEmpty>::Type;

template<typename List, template<typename T> class Pred, bool isEmpty = IsEmptyV<List>>
struct Filter;

template<typename List, template<typename T> class Pred>
struct Filter<List, Pred, true> {
  using Type = List;
};

template<typename List, template<typename T> class Pred>
struct Filter<List, Pred, false> {
private:
  using First = FrontT<List>;
  using Rest = PopFrontT<List>;
  using FilteredRest = typename Filter<Rest, Pred>::Type;

public:
  using Type = IfT<Pred<First>::value, PushFrontT<FilteredRest, FrontT<List>>, FilteredRest>;
};

template<typename List, template<typename T> class Pred>
using FilterT = typename Filter<List, Pred>::Type;

}  // namespace gtk