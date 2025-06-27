#pragma once

#include <iostream>
#include <utility>

#include "IntegerSequence.h"
#include "Typelist.h"


namespace gtk
{

template<typename... Elements>
struct Tuple;

template<>
struct Tuple<> {
};

template<typename Head, typename... Tail>
struct Tuple<Head, Tail...> {
  using ElementTypes = Typelist<Head, Tail...>;

  constexpr Tuple() = default;

  template<typename H, typename... Rest>
  constexpr Tuple(H&& h, const Tuple<Rest...>& t) : head(std::forward<H>(h)),
                                                    tail(t)
  {
  }


  template<typename H, typename... T, typename = std::enable_if_t<sizeof...(T) == sizeof...(Tail)>>
  constexpr Tuple(H&& h, T&&... t) : head(std::forward<H>(h)),
                                     tail(std::forward<T>(t)...)
  {
  }

  template<typename H, typename... T, typename = std::enable_if_t<sizeof...(T) == sizeof...(Tail)>>
  constexpr Tuple(const Tuple<H, T...>& other) : head(other.head),
                                                 tail(other.tail)
  {
  }


  Head head{};
  Tuple<Tail...> tail{};
};

template<typename... Elements>
Tuple(Elements...) -> Tuple<Elements...>;

template<typename... Elements>
constexpr auto MakeTuple(Elements&&... elements)
{
  return Tuple<std::decay_t<Elements>...>(std::forward<Elements>(elements)...);
}

template<typename T>
struct IntegerSequenceTupleHelper {
};

template<typename T, T... integers>
struct IntegerSequenceTupleHelper<IntegerSequence<T, integers...>> {
  using Type = Tuple<decltype(integers)...>;
  static constexpr auto value = Type{integers...};
};

template<typename T>
constexpr auto IntegerSequenceTuple()
{
  return IntegerSequenceTupleHelper<T>::value;
}


template<>
struct IsTypelistEmpty<Tuple<>> {
  static constexpr bool value = true;
};

template<typename Head, typename... Tail>
struct TypelistFront<Tuple<Head, Tail...>> {
  using Type = Head;
};

template<typename... Elements, typename NewElement>
struct TypelistPushFront<Tuple<Elements...>, NewElement> {
  using Type = Tuple<NewElement, Elements...>;
};

template<typename Head, typename... Tail>
struct TypelistPopFront<Tuple<Head, Tail...>> {
  using Type = Tuple<Tail...>;
};

template<typename... Elements, typename NewElement>
struct TypelistPushBack<Tuple<Elements...>, NewElement> {
  using Type = Tuple<Elements..., NewElement>;
};

template<size_t index, typename... Elements>
constexpr const GetType<Tuple<Elements...>, index>& Get(const Tuple<Elements...>& t)
{
  if constexpr (index == 0) {
    return t.head;
  } else {
    return Get<index - 1>(t.tail);
  }
}

template<size_t index, typename... Elements>
constexpr GetType<Tuple<Elements...>, index>& Get(Tuple<Elements...>& t)
{
  if constexpr (index == 0) {
    return t.head;
  } else {
    return Get<index - 1>(t.tail);
  }
}

template<size_t index, typename... Elements>
constexpr GetType<Tuple<Elements...>, index>&& Get(Tuple<Elements...>&& t)
{
  if constexpr (index == 0) {
    return t.head;
  } else {
    return Get<index - 1>(t.tail);
  }
}

template<size_t i, size_t j, size_t... rest, typename... Elements>
constexpr auto Get(const Tuple<Elements...>& t)
{
  return MakeTuple(Get<i>(t), Get<j>(t), Get<rest>(t)...);
}

template<typename I>
struct IndexedGetHelper {
};

template<size_t... indices>
struct IndexedGetHelper<IndexSequence<indices...>> {
  template<typename... Elements>
  static constexpr auto Impl(const Tuple<Elements...>& t)
  {
    return MakeTuple(Get<indices>(t)...);
  }
};

template<typename I, typename... Elements>
constexpr auto Get(const Tuple<Elements...>& t)
{
  return IndexedGetHelper<I>::Impl(t);
}

template<typename Head, typename... Elements>
constexpr auto& Get(Tuple<Head, Elements...>& t, size_t index)
{
  if (index == 0) {
    return t.head;
  }
  if constexpr (sizeof...(Elements) != 0) {
    return Get(t.tail, index - 1);
  } else {
    throw std::out_of_range("Index out of range in Tuple::Get");
  }
}

template<typename Head, typename... Elements>
constexpr const auto& Get(const Tuple<Head, Elements...>& t, size_t index)
{
  if (index == 0) {
    return t.head;
  }
  if constexpr (sizeof...(Elements) != 0) {
    return Get(t.tail, index - 1);
  } else {
    throw std::out_of_range("Index out of range in Tuple::Get");
  }
}

template<typename E, typename... Elements>
constexpr E Front(const Tuple<E, Elements...>& tuple)
{
  return tuple.head;
}

template<typename H, typename... Elements>
constexpr Tuple<H, Elements...> PushFront(const Tuple<Elements...>& t, const H& newElement)
{
  return Tuple<H, Elements...>{newElement, t};
}

template<typename H, typename... Elements>
constexpr Tuple<Elements...> PopFront(const Tuple<H, Elements...>& t)
{
  return t.tail;
}

template<typename E>
constexpr Tuple<E> PushBack(const Tuple<>&, const E& newElement)
{
  return Tuple<E>{newElement};
}

template<typename... Elements, typename NewElement>
constexpr auto PushBack(const Tuple<Elements...>& t, const NewElement& newElement)
{
  return Tuple<Elements..., NewElement>{t.head, PushBack(t.tail, newElement)};
}

template<typename H, typename... Elements>
constexpr auto PopBack(const Tuple<H, Elements...>& t)
{
  if constexpr (sizeof...(Elements) == 0) {
    return Tuple<>{};
  } else {
    return PushFront(PopBack(t.tail), t.head);
  }
}

template<typename... Elements>
constexpr auto Reverse(const Tuple<Elements...>& t)
{
  if constexpr (sizeof...(Elements) < 2) {
    return t;
  } else {
    return PushBack(Reverse(t.tail), t.head);
  }
}

template<typename H, typename... Elements>
constexpr auto Back(const Tuple<H, Elements...>& t)
{
  if constexpr (sizeof...(Elements) == 0) {
    return t.head;  // Only one element
  } else {
    return Back(tail(t));
  }
}

template<typename... Elements, typename F>
constexpr auto Transform(const Tuple<Elements...>& t, F&& f)
{
  if constexpr (sizeof...(Elements) == 0) {
    return Tuple<>{};
  } else {
    return Tuple<decltype(f(std::declval<Elements>()))...>{
      f(t.head), Transform(t.tail, std::forward<F>(f))
    };
  }
}

template<typename BinaryOp>
constexpr auto Transform(const Tuple<>&, const Tuple<>&, BinaryOp&&)
{
  return Tuple<>{};
}

template<typename H1, typename H2, typename BinaryOp>
constexpr auto Transform(const Tuple<H1>& t1, const Tuple<H2>& t2, BinaryOp&& op)
{
  return Tuple<decltype(op(t1.head, t2.head))>{op(t1.head, t2.head)};
}

template<typename H1, typename... E1, typename H2, typename... E2, typename BinaryOp>
constexpr auto Transform(const Tuple<H1, E1...>& t1, const Tuple<H2, E2...>& t2, BinaryOp&& op)
{
  static_assert(sizeof...(E1) == sizeof...(E2), "Tuples must have the same size for Transform");
  return PushFront(
    Transform(t1.tail, t2.tail, std::forward<BinaryOp>(op)),
    std::forward<BinaryOp>(op)(t1.head, t2.head)
  );
}

template<typename... Elements, typename F, typename I>
constexpr auto FoldLeft(const Tuple<Elements...>& t, F&& op, I&& init)
{
  if constexpr (sizeof...(Elements) == 0) {
    return init;
  } else {
    return FoldLeft(t.tail, std::forward<F>(op), op(init, t.head));
  }
}

template<typename... Elements, typename F, typename I>
constexpr auto FoldRight(const Tuple<Elements...>& t, F&& op, I&& init)
{
  if constexpr (sizeof...(Elements) == 0) {
    return init;
  } else {
    return op(t.head, FoldRight(t.tail, std::forward<F>(op), std::forward<I>(init)));
  }
}

template<typename... Elements>
constexpr size_t Size(const Tuple<Elements...>&)
{
  return sizeof...(Elements);
}

template<typename... Elements>
constexpr bool IsEmpty(const Tuple<Elements...>&)
{
  return sizeof...(Elements) == 0;
}

// TODO: Implement compile-time filtering for tuples


template<typename... E1, typename... E2>
constexpr bool operator==(const Tuple<E1...>& lhs, const Tuple<E2...>& rhs)
{
  if constexpr (sizeof...(E1) != sizeof...(E2)) {
    return false;
  } else if constexpr (sizeof...(E1) == 0) {
    return true;
  } else {
    return (lhs.head == rhs.head) && (lhs.tail == rhs.tail);
  }
}

template<typename... E1, typename... E2>
constexpr bool operator!=(const Tuple<E1...>& lhs, const Tuple<E2...>& rhs)
{
  return !(lhs == rhs);
}


template<typename... E1, typename... E2>
constexpr bool operator<(const Tuple<E1...>& lhs, const Tuple<E2...>& rhs)
{
  if constexpr (sizeof...(E1) != sizeof...(E2)) {
    return sizeof...(E1) < sizeof...(E2);
  } else if constexpr (sizeof...(E1) == 0) {
    return false;
  } else {
    return (lhs.head < rhs.head) || (lhs.head == rhs.head && lhs.tail < rhs.tail);
  }
}

template<typename... E1, typename... E2>
constexpr bool operator>(const Tuple<E1...>& lhs, const Tuple<E2...>& rhs)
{
  return rhs < lhs;
}

template<typename... E1, typename... E2>
constexpr bool operator<=(const Tuple<E1...>& lhs, const Tuple<E2...>& rhs)
{
  return !(rhs < lhs);
}

template<typename... E1, typename... E2>
constexpr bool operator>=(const Tuple<E1...>& lhs, const Tuple<E2...>& rhs)
{
  return !(lhs < rhs);
}


inline std::ostream& Print(std::ostream& os, const Tuple<>&, bool first = true)
{
  return os << (first ? "(" : ")");
}

template<typename Head, typename... Tail>
std::ostream& Print(std::ostream& os, const Tuple<Head, Tail...>& tuple, bool first = true)
{
  os << (first ? "(" : ", ");
  os << tuple.head;
  return Print(os, tuple.tail, false);
}

template<typename... Elements>
std::ostream& operator<<(std::ostream& os, const Tuple<Elements...>& tuple)
{
  return Print(os, tuple);
}


}  // namespace gtk