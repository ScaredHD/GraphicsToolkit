#include <iostream>
#include <string>

#include "GraphicsToolkit/core/Tuple.h"
#include "GraphicsToolkit/math/Tensor.h"


using namespace gtk;

int main()
{

  constexpr auto t = gtk::Tuple<int, double, double>{1, 2.5, 3.14};

  static_assert(Get<2, 2, 2>(t) == gtk::MakeTuple(3.14, 3.14, 3.14));
  static_assert(Get<1, 2, 0>(t) == gtk::MakeTuple(2.5, 3.14, 1));
  static_assert(std::is_same_v<
                IntegerSequenceGenerator<size_t, 0, 9, 2>, IntegerSequence<size_t, 0, 2, 4, 6, 8>>);
  static_assert(
    std::is_same_v<IntegerSequenceGenerator<size_t, 0, 9, 3>, IntegerSequence<size_t, 0, 3, 6, 9>>
  );
  static_assert(
    std::is_same_v<IntegerSequenceGenerator<size_t, 0, 10, 3>, IntegerSequence<size_t, 0, 3, 6, 9>>
  );
  static_assert(
    std::is_same_v<IntegerSequenceGenerator<size_t, 1, 10, 3>, IntegerSequence<size_t, 1, 4, 7, 10>>
  );

  static_assert(std::is_same_v<
                IntegerSequenceGenerator<size_t, 10, 0, -1>,
                IntegerSequence<size_t, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0>>);

  static_assert(std::is_same_v<
                IntegerSequenceGenerator<int, 8, -3, -2>, IntegerSequence<int, 8, 6, 4, 2, 0, -2>>);


  static_assert(Get<MakeIndexSequence<3>>(t) == t);
  static_assert(Get<IntegerSequenceGenerator<size_t, 2, 0, -1>>(t) == Reverse(t));

  constexpr auto x = IntegerSequenceTuple<IntegerSequence<int, 1, 2, 3>>();
  static_assert(std::is_same_v<decltype(x), const Tuple<int, int, int>>);
  static_assert(x == gtk::MakeTuple(1, 2, 3));


  {
    constexpr Tensor<int, 4, 3> t{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    constexpr auto x = t(1, 2);
  }
  {
    Tensor<int, 4, 3> t{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto x = t(1, 2);
  }
  {
    constexpr Tensor<int, 4, 3> t{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        std::cout << t(i, j) << ", ";
      }
      std::cout << "\n";
    }
    for (size_t i = 0; i < decltype(t)::DimensionType::count; ++i) {
      std::cout << decltype(t)::DimensionType::UnflattenedIndex(i) << ", ";
    }
    std::cout << "\n";
  }
}