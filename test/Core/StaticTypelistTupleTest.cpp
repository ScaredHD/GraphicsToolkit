#include <gtest/gtest.h>

#include <iostream>

#include "Tuple.h"
#include "Typelist.h"


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
struct IsOdd {
  static constexpr bool value = T::value % 2 != 0;
};

using namespace gtk;

void test()
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

    using T0 = FilterType<Integers, IsOdd>;


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
}

TEST(Core, TypelistTuple)
{
  test();
}