#pragma once

#include <type_traits>

namespace gtk
{


template<typename T, T... values>
struct IntegerSequence {
  using ElementType = T;
};

template<typename T>
struct IsIntegerSequenceEmpty {
  static constexpr bool value = false;
};

template<typename T>
struct IsIntegerSequenceEmpty<IntegerSequence<T>> {
  static constexpr bool value = true;
};

template<typename T>
inline constexpr bool IsIntegerSequenceEmptyV = IsIntegerSequenceEmpty<T>::value;

template<size_t index, typename T, T Head, T... Tail>
struct IntegerSequenceGetHelper {
  static_assert(index < sizeof...(Tail) + 1, "Index out of bounds");
  using Type = T;
  static constexpr T value = IntegerSequenceGetHelper<index - 1, T, Tail...>::value;
};

template<typename T, T Head, T... Tail>
struct IntegerSequenceGetHelper<0, T, Head, Tail...> {
  using Type = T;
  static constexpr T value = Head;
};

template<typename S, size_t index>
struct IntegerSequenceGet {
};

template<size_t index, typename T, T Head, T... Tail>
struct IntegerSequenceGet<IntegerSequence<T, Head, Tail...>, index> {
  using Type = typename IntegerSequenceGetHelper<index, T, Head, Tail...>::Type;
  static constexpr T value = IntegerSequenceGetHelper<index, T, Head, Tail...>::value;
};

template<typename S, size_t index>
inline constexpr typename IntegerSequenceGet<S, index>::Type IntegerSequenceGetV =
  IntegerSequenceGet<S, index>::value;


template<typename T, T Head, T...>
struct IntegerSequenceFront {
  using Type = T;
  static constexpr T value = Head;
};

template<typename List, typename T, T val>
struct IntegerSequencePushFront;

template<typename T, T... values, T newValue>
struct IntegerSequencePushFront<IntegerSequence<T, values...>, T, newValue> {
  using Type = IntegerSequence<T, newValue, values...>;
};

template<typename List, typename T, T val>
using IntegerSequencePushFrontT = typename IntegerSequencePushFront<List, T, val>::Type;


template<typename List>
struct IntegerSequencePopFront;

template<typename T, T Head, T... Tail>
struct IntegerSequencePopFront<IntegerSequence<T, Head, Tail...>> {
  using Type = IntegerSequence<T, Tail...>;
};

template<typename List>
using IntegerSequencePopFrontT = typename IntegerSequencePopFront<List>::Type;

template<typename List, typename T, T val>
struct IntegerSequencePushBack;

template<typename T, T... values, T newValue>
struct IntegerSequencePushBack<IntegerSequence<T, values...>, T, newValue> {
  using Type = IntegerSequence<T, values..., newValue>;
};

template<typename Seq, bool isEmpty = IsIntegerSequenceEmptyV<Seq>>
struct IntegerSequenceReverse;

template<typename T>
struct IntegerSequenceReverse<IntegerSequence<T>, true> {
  using Type = IntegerSequence<T>;
};

template<typename T, T head, T... values>
struct IntegerSequenceReverse<IntegerSequence<T, head, values...>, false> {
  using Type = typename IntegerSequencePushBack<
    typename IntegerSequenceReverse<IntegerSequence<T, values...>>::Type,
    T,
    head>::Type;
};

template<typename S>
using IntegerSequenceReverseT = typename IntegerSequenceReverse<S>::Type;

template<typename S>
struct IntegerSequencePopBack {
  using Type = typename IntegerSequenceReverse<
    typename IntegerSequencePopFront<typename IntegerSequenceReverse<S>::Type>::Type>::Type;
};

template<typename S>
using IntegerSequencePopBackT = typename IntegerSequencePopBack<S>::Type;

template<typename S, typename F>
struct IntegerSequenceTransform;

template<typename T, typename F, T... values>
struct IntegerSequenceTransform<IntegerSequence<T, values...>, F> {
  using Type = IntegerSequence<T, F{}(values)...>;
};

template<typename T, T current, T target, int stride, typename = std::enable_if_t<stride != 0>>
struct IntegerSequenceTraits {
  static constexpr bool isIncreasing = (stride > 0);
  static constexpr bool isLast =
    (isIncreasing ? target - current < stride : current - target < -stride);
};

template<typename T, T current, T target, int stride, bool isLast, T... accValues>
struct IntegerSequenceGeneratorHelper {
  using Type = typename IntegerSequenceGeneratorHelper<
    T,
    current + stride,
    target,
    stride,
    IntegerSequenceTraits<T, current + stride, target, stride>::isLast,
    accValues...,
    current>::Type;
};

template<typename T, T current, T target, int stride, T... accValues>
struct IntegerSequenceGeneratorHelper<T, current, target, stride, true, accValues...> {
  using Type = IntegerSequence<T, accValues..., current>;
};

template<typename T, T current, T target, int stride>
using IntegerSequenceGenerator = typename IntegerSequenceGeneratorHelper<
  T,
  current,
  target,
  stride,
  IntegerSequenceTraits<T, current, target, stride>::isLast>::Type;


template<typename T, size_t count>
struct MakeIntegerSequence {
private:
  static constexpr T first = 0;
  static constexpr T last = static_cast<T>(count - 1);
  static constexpr T stride = 1;

public:
  using Type = IntegerSequenceGenerator<T, first, last, stride>;
};

template<typename T>
struct MakeIntegerSequence<T, 0> {
  using Type = IntegerSequence<T>;
};

template<size_t... indices>
using IndexSequence = IntegerSequence<size_t, indices...>;

template<size_t count>
using MakeIndexSequence = typename MakeIntegerSequence<size_t, count>::Type;

template<typename List, template<typename U = typename List::ElementType, U x = U{}> class Pred>
struct IntegerSequenceFilter;

template<typename T, template<typename U, U> class Pred>
struct IntegerSequenceFilter<IntegerSequence<T>, Pred> {
  using Type = IntegerSequence<T>;
};

template<typename T, T head, T... values, template<typename U, U> class Pred>
struct IntegerSequenceFilter<IntegerSequence<T, head, values...>, Pred> {
  using FilteredTail = typename IntegerSequenceFilter<IntegerSequence<T, values...>, Pred>::Type;
  using Type = std::conditional_t<
    Pred<T, head>::value,
    IntegerSequencePushFrontT<FilteredTail, T, head>,
    FilteredTail>;
};

template<typename List, template<typename U = typename List::ElementType, U x = U{}> class Pred>
using IntegerSequenceFilterT = typename IntegerSequenceFilter<List, Pred>::Type;

template<typename T, T x>
struct IsEven {
  static constexpr bool value = (x % 2 == 0);
};

template<typename T, T x>
struct IsOdd {
  static constexpr bool value = (x % 2 != 0);
};

template<typename T, T x>
struct CompareWith {
  template<T y>
  struct Equal {
    static constexpr bool value = (x == y);
  };
  template<T y>
  struct NotEqual {
    static constexpr bool value = (x != y);
  };
  template<T y>
  struct LessThan {
    static constexpr bool value = (x < y);
  };
  template<T y>
  struct GreaterThan {
    static constexpr bool value = (x > y);
  };
  template<T y>
  struct LessThanOrEqual {
    static constexpr bool value = (x <= y);
  };
  template<T y>
  struct GreaterThanOrEqual {
    static constexpr bool value = (x >= y);
  };
};


}  // namespace gtk