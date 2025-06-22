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

template<typename... Elements>
struct Typelist {
};

template<typename T>
struct IsTypelistEmpty {
  static constexpr bool value = false;
};

template<>
struct IsTypelistEmpty<Typelist<>> {
  static constexpr bool value = true;
};


template<typename List>
struct TypelistFront {
};

template<typename Head, typename... Tail>
struct TypelistFront<Typelist<Head, Tail...>> {
  using Type = Head;
};

template<typename List, typename NewElement>
struct TypelistPushFront {
};

template<typename... Elements, typename NewElement>
struct TypelistPushFront<Typelist<Elements...>, NewElement> {
  using Type = Typelist<NewElement, Elements...>;
};

template<typename List>
struct TypelistPopFront {
  using Type = List;
};

template<typename Head, typename... Tail>
struct TypelistPopFront<Typelist<Head, Tail...>> {
  using Type = Typelist<Tail...>;
};

// Aliases

template<typename T>
inline constexpr bool IsTypelistEmptyV = IsTypelistEmpty<T>::value;

template<typename T>
using FrontType = typename TypelistFront<T>::Type;

template<typename T, typename NewElement>
using PushFrontType = typename TypelistPushFront<T, NewElement>::Type;

template<typename T>
using PopFrontType = typename TypelistPopFront<T>::Type;

// Following traits derive from the above definitions

template<typename T>
struct TypelistSize {
  static constexpr size_t value = IsTypelistEmptyV<T> ? 0 : 1 + TypelistSize<PopFrontType<T>>::value;
};

template<typename T>
using SizeV = typename TypelistSize<T>::value;

// TODO: Add out of bounds checks for Get
template<typename T, size_t index>
struct TypelistGet {
  using Type = typename TypelistGet<PopFrontType<T>, index - 1>::Type;
};

template<typename T>
struct TypelistGet<T, 0> {
  using Type = FrontType<T>;
};

template<typename T, size_t index>
using GetType = typename TypelistGet<T, index>::Type;

// Optimization available, need customization for specific types
template<typename T, typename NewElement, bool isEmpty = IsTypelistEmptyV<T>>
struct PushBackHelper;

template<typename T, typename NewElement>
struct PushBackHelper<T, NewElement, true> {
  using Type = PushFrontType<T, NewElement>;
};

template<typename T, typename NewElement>
struct PushBackHelper<T, NewElement, false> {
  using Type =
    PushFrontType<typename PushBackHelper<PopFrontType<T>, NewElement>::Type, FrontType<T>>;
};

template<typename T, typename NewElement>
struct TypelistPushBack : PushBackHelper<T, NewElement> {
};

template<typename T, typename NewElement>
using PushBackType = typename TypelistPushBack<T, NewElement>::Type;


template<typename T, bool isEmpty = IsTypelistEmptyV<T>>
struct TypelistReverse;

template<typename T>
struct TypelistReverse<T, true> {
  using Type = T;
};

template<typename T>
struct TypelistReverse<T, false> {
  using Type = PushBackType<typename TypelistReverse<PopFrontType<T>>::Type, FrontType<T>>;
};

template<typename T>
using ReverseType = typename TypelistReverse<T>::Type;

template<typename T, bool isEmpty = IsTypelistEmptyV<T>>
struct TypelistPopBack;

template<typename T>
struct TypelistPopBack<T, true> {
  using Type = T;
};

template<typename T>
struct TypelistPopBack<T, false> {
  using Type = ReverseType<PopFrontType<ReverseType<T>>>;
};

template<typename T>
using PopBackType = typename TypelistPopBack<T>::Type;

template<typename List>
struct TypelistBack {
  using Type = FrontType<ReverseType<List>>;
};

template<typename List>
using BackType = typename TypelistBack<List>::Type;

template<typename List, template<typename T> class F, bool isEmpty = IsTypelistEmptyV<List>>
struct TypelistTransform;

template<typename List, template<typename T> class F>
struct TypelistTransform<List, F, true> {
  using Type = List;
};

template<typename List, template<typename T> class F>
struct TypelistTransform<List, F, false>
    : TypelistPushFront<
        typename TypelistTransform<PopFrontType<List>, F>::Type,
        typename F<FrontType<List>>::Type> {
};

template<typename List, template<typename T> class F>
using TransformType = typename TypelistTransform<List, F>::Type;


template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsTypelistEmptyV<List>>
struct TypelistFoldLeft;

template<typename List, template<typename T, typename U> class F, typename I>
struct TypelistFoldLeft<List, F, I, true> {
  using Type = I;
};

template<typename List, template<typename T, typename U> class F, typename I>
struct TypelistFoldLeft<List, F, I, false> {
  using Type =
    typename F<typename TypelistFoldLeft<PopBackType<List>, F, I>::Type, BackType<List>>::Type;
};

template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsTypelistEmptyV<List>>
using FoldLeftType = typename TypelistFoldLeft<List, F, I, isEmpty>::Type;

template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsTypelistEmptyV<List>>
struct TypelistFoldRight;

template<typename List, template<typename T, typename U> class F, typename I>
struct TypelistFoldRight<List, F, I, true> {
  using Type = I;
};

template<typename List, template<typename T, typename U> class F, typename I>
struct TypelistFoldRight<List, F, I, false> {
  using Type =
    typename F<FrontType<List>, typename TypelistFoldRight<PopFrontType<List>, F, I>::Type>::Type;
};

template<
  typename List,
  template<typename T, typename U> class F,
  typename I,
  bool isEmpty = IsTypelistEmptyV<List>>
using FoldRightType = typename TypelistFoldLeft<List, F, I, isEmpty>::Type;

template<typename List, template<typename T> class Pred, bool isEmpty = IsTypelistEmptyV<List>>
struct TypelistFilter;

template<typename List, template<typename T> class Pred>
struct TypelistFilter<List, Pred, true> {
  using Type = List;
};

template<typename List, template<typename T> class Pred>
struct TypelistFilter<List, Pred, false> {
private:
  using First = FrontType<List>;
  using Rest = PopFrontType<List>;
  using FilteredRest = typename TypelistFilter<Rest, Pred>::Type;

public:
  using Type = IfT<Pred<First>::value, PushFrontType<FilteredRest, FrontType<List>>, FilteredRest>;
};

template<typename List, template<typename T> class Pred>
using FilterType = typename TypelistFilter<List, Pred>::Type;


}  // namespace gtk