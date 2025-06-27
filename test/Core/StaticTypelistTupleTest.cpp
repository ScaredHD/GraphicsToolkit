#include <gtest/gtest.h>
#include <iostream>

#include "IntegerSequence.h"
#include "Tuple.h"
#include "Typelist.h"

namespace
{

template<typename T>
struct AddConst {
  using Type = const T;
};

template<int i>
struct Int {
  static constexpr int value = i;
};

template<typename T, typename U>
struct Add {
  using Type = Int<T::value + U::value>;
};

template<typename T>
struct IsOddType {
  static constexpr bool value = T::value % 2 != 0;
};

// Predicate templates for testing Equals
template<typename T, T val>
using EqualsZero = typename gtk::Equals<T, val>::template Int<0>;

template<typename T, T val>
using EqualsOne = typename gtk::Equals<T, val>::template Int<1>;

template<typename T, T val>
using EqualsTwo = typename gtk::Equals<T, val>::template Int<2>;

template<typename T, T val>
using EqualsFive = typename gtk::Equals<T, val>::template Int<5>;

template<typename T, T val>
using EqualsNegOne = typename gtk::Equals<T, val>::template Int<-1>;

}  // namespace
using namespace gtk;

static void test()
{
  {


    using Types = gtk::Typelist<char, short, int, long, float, double>;
    using EmptyList = gtk::Typelist<>;

    using A0 = PushFrontType<Types, void>;
    using A1 = PushFrontType<EmptyList, void>;

    using B0 = PopFrontType<Types>;
    // using B1 = PopFrontType<EmptyList>;

    using C0 = FrontType<Types>;
    // using C1 = FrontType<EmptyList>;

    bool D0 = IsTypelistEmptyV<Types>;
    bool D1 = IsTypelistEmptyV<EmptyList>;

    using E0 = PushBackType<Types, void>;
    using E1 = PushBackType<EmptyList, void>;

    using F0 = PopBackType<Types>;
    using F1 = PopBackType<EmptyList>;

    using G0 = ReverseType<Types>;
    using G1 = ReverseType<EmptyList>;
    static_assert(IsTypelistEmptyV<G1>, "Empty list should remain empty after reverse");

    using H0 = BackType<Types>;
    // using H1 = BackType<EmptyList>;

    using I0 = TransformType<Types, AddConst>;
    using I1 = TransformType<EmptyList, AddConst>;
    static_assert(
      std::is_same_v<
        I0, Typelist<const char, const short, const int, const long, const float, const double>>,
      "Transform should add const to each type in the list"
    );

    using Integers = Typelist<Int<1>, Int<2>, Int<3>, Int<4>>;
    using S0 = FoldLeftType<Integers, Add, Int<0>>;
    using S1 = FoldRightType<Integers, Add, Int<0>>;
    using S2 = FoldLeftType<Integers, Add, Int<5>>;
    using S3 = FoldRightType<Integers, Add, Int<5>>;

    static_assert(std::is_same_v<S0, Int<10>>, "FoldLeft should sum integers in the list");
    static_assert(std::is_same_v<S1, Int<10>>, "FoldRight should sum integers in the list");
    static_assert(std::is_same_v<S2, Int<15>>, "FoldLeft with initial value should sum correctly");
    static_assert(std::is_same_v<S3, Int<15>>, "FoldRight with initial value should sum correctly");

    using T0 = FilterType<Integers, IsOddType>;


    constexpr Tuple<int, double, char> t(42, 3.14, 'a');
    ReverseType<Tuple<int, double, char>> reversedTuple('a', 3.14, 42);

    constexpr GetType<decltype(t)::ElementTypes, 1> secondElement = Get<1>(t);

    using U0 = PushBackType<Tuple<int, double>, char>;
    using U1 = GetType<U0, 0>;

    constexpr Tuple<int, double, char> t2 = t;

    auto t3 = PushBack(t2, 'b');

    constexpr Tuple<int, int, int> t4{1, 2, 3};
    auto addOne = [](auto x) { return x + 1; };
    constexpr auto t5 = Transform(t4, addOne);
    static_assert(t5 == Tuple<int, int, int>{2, 3, 4}, "Transform should add one to each element");

    static_assert(
      Tuple{1, 2, 3} < Tuple{1, 3, 2}, "Tuple comparison should work lexicographically"
    );
    static_assert(
      !(Tuple{2, 1, 3} < Tuple{1, 1, 4}), "Tuple comparison should work lexicographically"
    );
    static_assert(Tuple{3, 1} < Tuple{1, 2, 3}, "Tuple comparison should work lexicographically");
  }
  {

    constexpr auto t = gtk::Tuple<int, double, double>{1, 2.5, 3.14};

    static_assert(Get<2, 2, 2>(t) == gtk::MakeTuple(3.14, 3.14, 3.14));
    static_assert(Get<1, 2, 0>(t) == gtk::MakeTuple(2.5, 3.14, 1));
    static_assert(std::is_same_v<
                  IntegerSequenceGenerator<size_t, 0, 9, 2>, IntegerSequence<size_t, 0, 2, 4, 6, 8>>
    );
    static_assert(
      std::is_same_v<IntegerSequenceGenerator<size_t, 0, 9, 3>, IntegerSequence<size_t, 0, 3, 6, 9>>
    );
    static_assert(std::is_same_v<
                  IntegerSequenceGenerator<size_t, 0, 10, 3>, IntegerSequence<size_t, 0, 3, 6, 9>>);
    static_assert(std::is_same_v<
                  IntegerSequenceGenerator<size_t, 1, 10, 3>, IntegerSequence<size_t, 1, 4, 7, 10>>
    );

    static_assert(std::is_same_v<
                  IntegerSequenceGenerator<size_t, 10, 0, -1>,
                  IntegerSequence<size_t, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0>>);

    static_assert(std::is_same_v<
                  IntegerSequenceGenerator<int, 8, -3, -2>, IntegerSequence<int, 8, 6, 4, 2, 0, -2>>
    );
  }
  {
    // Test IntegerSequenceFilter - currently has implementation issues
    // Basic predicate tests work fine
    static_assert(IsOdd<int, 1>::value);
    static_assert(!IsOdd<int, 2>::value);
    static_assert(IsEven<int, 2>::value);
    static_assert(!IsEven<int, 1>::value);

    static_assert(
      std::is_same_v<IntegerSequenceFilter<IntegerSequence<int>, IsOdd>::Type, IntegerSequence<int>>
    );

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 1, 2, 3, 4, 5>, IsOdd>::Type,
                  IntegerSequence<int, 1, 3, 5>>);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 2, 4, 6>, IsEven>::Type,
                  IntegerSequence<int, 2, 4, 6>>);

    // For now, just verify that the types can be instantiated
    using TestFilter1 = IntegerSequenceFilter<IntegerSequence<int>, IsOdd>;
    using TestFilter2 = IntegerSequenceFilter<IntegerSequence<int, 1, 3>, IsOdd>;

    // Basic sanity check - ensure IntegerSequence itself works
    static_assert(std::is_same_v<IntegerSequence<int>, IntegerSequence<int>>);
    static_assert(std::is_same_v<IntegerSequence<int, 1, 3>, IntegerSequence<int, 1, 3>>);

    // More comprehensive IntegerSequenceFilter tests

    // Test single element filtering
    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 1>, IsOdd>::Type, IntegerSequence<int, 1>>
    );

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 2>, IsOdd>::Type, IntegerSequence<int>>
    );

    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 4>, IsEven>::Type, IntegerSequence<int, 4>>
    );

    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 3>, IsEven>::Type, IntegerSequence<int>>
    );

    // Test alternating patterns
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 1, 2, 3, 4>, IsOdd>::Type,
                  IntegerSequence<int, 1, 3>>);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 1, 2, 3, 4>, IsEven>::Type,
                  IntegerSequence<int, 2, 4>>);

    // Test all odd sequence
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 1, 3, 5, 7, 9>, IsOdd>::Type,
                  IntegerSequence<int, 1, 3, 5, 7, 9>>);

    // Test all even sequence
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 2, 4, 6, 8>, IsEven>::Type,
                  IntegerSequence<int, 2, 4, 6, 8>>);

    // Test filtering all odd from even sequence
    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 2, 4, 6, 8>, IsOdd>::Type, IntegerSequence<int>>
    );

    // Test filtering all even from odd sequence
    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 1, 3, 5, 7>, IsEven>::Type, IntegerSequence<int>>
    );

    // Test with different types
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<size_t, 0, 1, 2, 3, 4, 5>, IsEven>::Type,
                  IntegerSequence<size_t, 0, 2, 4>>);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<long, 10, 11, 12, 13>, IsOdd>::Type,
                  IntegerSequence<long, 11, 13>>);

    // Test with negative numbers
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, -3, -2, -1, 0, 1, 2, 3>, IsOdd>::Type,
                  IntegerSequence<int, -3, -1, 1, 3>>);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, -4, -2, 0, 2, 4>, IsEven>::Type,
                  IntegerSequence<int, -4, -2, 0, 2, 4>>);

    // Test larger sequences
    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>, IsOdd>::Type,
        IntegerSequence<int, 1, 3, 5, 7, 9>>
    );

    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>, IsEven>::Type,
        IntegerSequence<int, 0, 2, 4, 6, 8>>
    );

    // Test Equals predicate
    // Test basic Equals functionality
    static_assert(EqualsZero<int, 0>::value);
    static_assert(!EqualsZero<int, 1>::value);
    static_assert(EqualsOne<int, 1>::value);
    static_assert(!EqualsOne<int, 0>::value);

    // Test filtering with Equals predicates
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 0, 1, 2, 0, 3, 0>, EqualsZero>::Type,
                  IntegerSequence<int, 0, 0, 0>>);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 1, 2, 1, 3, 1, 4>, EqualsOne>::Type,
                  IntegerSequence<int, 1, 1, 1>>);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 0, 1, 2, 3, 4>, EqualsTwo>::Type,
                  IntegerSequence<int, 2>>);

    // Test empty result
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 1, 2, 3, 4>, EqualsZero>::Type,
                  IntegerSequence<int>>);

    // Test with no matches
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, 0, 1, 2, 3, 4>, EqualsFive>::Type,
                  IntegerSequence<int>>);

    // Test with single element match
    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 5>, EqualsFive>::Type, IntegerSequence<int, 5>>
    );

    // Test with single element no match
    static_assert(
      std::is_same_v<
        IntegerSequenceFilter<IntegerSequence<int, 4>, EqualsFive>::Type, IntegerSequence<int>>
    );

    // Test with different types
    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<size_t, 0, 1, 0, 2, 0>, EqualsZero>::Type,
                  IntegerSequence<size_t, 0, 0, 0>>);

    // Test with negative numbers
    static_assert(EqualsNegOne<int, -1>::value);
    static_assert(!EqualsNegOne<int, 1>::value);

    static_assert(std::is_same_v<
                  IntegerSequenceFilter<IntegerSequence<int, -1, 0, -1, 1, -1>, EqualsNegOne>::Type,
                  IntegerSequence<int, -1, -1, -1>>);
  }
}

TEST(Core, TypelistTuple)
{
  test();
}