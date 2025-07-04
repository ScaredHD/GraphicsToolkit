#include <gtest/gtest.h>

#include "IntegerSequence.h"

TEST(Core, IntegerSequence)
{
  {
    // Basic IntegerSequence Tests
    using EmptySeq = gtk::IntegerSequence<int>;
    using IntSeq = gtk::IntegerSequence<int, 1, 2, 3>;
    using SizeSeq = gtk::IntegerSequence<size_t, 10, 20, 30>;

    // Test type aliases
    static_assert(std::is_same_v<IntSeq::ElementType, int>);
    static_assert(std::is_same_v<SizeSeq::ElementType, size_t>);
    static_assert(std::is_same_v<EmptySeq::ElementType, int>);
  }

  {
    // Empty Sequence Tests
    using EmptyInt = gtk::IntegerSequence<int>;
    using EmptySize = gtk::IntegerSequence<size_t>;
    using NonEmptySeq = gtk::IntegerSequence<int, 1, 2, 3>;

    static_assert(gtk::IsIntegerSequenceEmptyV<EmptyInt>);
    static_assert(gtk::IsIntegerSequenceEmptyV<EmptySize>);
    static_assert(!gtk::IsIntegerSequenceEmptyV<NonEmptySeq>);
  }

  {
    // Element Access Tests
    using TestSeq = gtk::IntegerSequence<int, 10, 20, 30, 40>;

    // Test IntegerSequenceGet
    static_assert(gtk::IntegerSequenceGetV<TestSeq, 0> == 10);
    static_assert(gtk::IntegerSequenceGetV<TestSeq, 1> == 20);
    static_assert(gtk::IntegerSequenceGetV<TestSeq, 2> == 30);
    static_assert(gtk::IntegerSequenceGetV<TestSeq, 3> == 40);

    // Test IntegerSequenceFront
    static_assert(gtk::IntegerSequenceFront<int, 10, 20, 30>::value == 10);
    static_assert(gtk::IntegerSequenceFront<size_t, 100, 200>::value == 100);
  }

  {
    // Push Front Tests
    using OriginalSeq = gtk::IntegerSequence<int, 2, 3, 4>;
    using PushedSeq = gtk::IntegerSequencePushFrontT<OriginalSeq, int, 1>;
    using ExpectedSeq = gtk::IntegerSequence<int, 1, 2, 3, 4>;

    static_assert(std::is_same_v<PushedSeq, ExpectedSeq>);

    // Test push front to empty sequence
    using EmptySeq = gtk::IntegerSequence<int>;
    using PushedEmpty = gtk::IntegerSequencePushFrontT<EmptySeq, int, 42>;
    using ExpectedSingle = gtk::IntegerSequence<int, 42>;

    static_assert(std::is_same_v<PushedEmpty, ExpectedSingle>);
  }

  {
    // Pop Front Tests
    using OriginalSeq = gtk::IntegerSequence<int, 1, 2, 3, 4>;
    using PoppedSeq = gtk::IntegerSequencePopFrontT<OriginalSeq>;
    using ExpectedSeq = gtk::IntegerSequence<int, 2, 3, 4>;

    static_assert(std::is_same_v<PoppedSeq, ExpectedSeq>);

    // Test pop front single element
    using SingleSeq = gtk::IntegerSequence<int, 42>;
    using PoppedSingle = gtk::IntegerSequencePopFrontT<SingleSeq>;
    using ExpectedEmpty = gtk::IntegerSequence<int>;

    static_assert(std::is_same_v<PoppedSingle, ExpectedEmpty>);
  }

  {
    // Push Back Tests
    using OriginalSeq = gtk::IntegerSequence<int, 1, 2, 3>;
    using PushedSeq = gtk::IntegerSequencePushBack<OriginalSeq, int, 4>::Type;
    using ExpectedSeq = gtk::IntegerSequence<int, 1, 2, 3, 4>;

    static_assert(std::is_same_v<PushedSeq, ExpectedSeq>);

    // Test push back to empty sequence
    using EmptySeq = gtk::IntegerSequence<int>;
    using PushedEmpty = gtk::IntegerSequencePushBack<EmptySeq, int, 42>::Type;
    using ExpectedSingle = gtk::IntegerSequence<int, 42>;

    static_assert(std::is_same_v<PushedEmpty, ExpectedSingle>);
  }

  {
    // Reverse Tests
    using OriginalSeq = gtk::IntegerSequence<int, 1, 2, 3, 4>;
    using ReversedSeq = gtk::IntegerSequenceReverseT<OriginalSeq>;
    using ExpectedSeq = gtk::IntegerSequence<int, 4, 3, 2, 1>;

    static_assert(std::is_same_v<ReversedSeq, ExpectedSeq>);

    // Test reverse single element
    using SingleSeq = gtk::IntegerSequence<int, 42>;
    using ReversedSingle = gtk::IntegerSequenceReverseT<SingleSeq>;

    static_assert(std::is_same_v<ReversedSingle, SingleSeq>);

    // Test reverse empty sequence
    using EmptySeq = gtk::IntegerSequence<int>;
    using ReversedEmpty = gtk::IntegerSequenceReverseT<EmptySeq>;

    static_assert(std::is_same_v<ReversedEmpty, EmptySeq>);
  }

  {
    // Pop Back Tests
    using OriginalSeq = gtk::IntegerSequence<int, 1, 2, 3, 4>;
    using PoppedSeq = gtk::IntegerSequencePopBackT<OriginalSeq>;
    using ExpectedSeq = gtk::IntegerSequence<int, 1, 2, 3>;

    static_assert(std::is_same_v<PoppedSeq, ExpectedSeq>);

    // Test pop back single element
    using SingleSeq = gtk::IntegerSequence<int, 42>;
    using PoppedSingle = gtk::IntegerSequencePopBackT<SingleSeq>;
    using ExpectedEmpty = gtk::IntegerSequence<int>;

    static_assert(std::is_same_v<PoppedSingle, ExpectedEmpty>);
  }

  {
    // Sequence Generation Tests
    using Generated0 = gtk::MakeIntegerSequence<int, 0>::Type;
    using Generated1 = gtk::MakeIntegerSequence<int, 1>::Type;
    using Generated2 = gtk::MakeIntegerSequence<int, 2>::Type;
    using Generated3 = gtk::MakeIntegerSequence<int, 3>::Type;
    using Generated5 = gtk::MakeIntegerSequence<int, 5>::Type;

    static_assert(std::is_same_v<Generated0, gtk::IntegerSequence<int>>);
    static_assert(std::is_same_v<Generated1, gtk::IntegerSequence<int, 0>>);
    static_assert(std::is_same_v<Generated2, gtk::IntegerSequence<int, 0, 1>>);
    static_assert(std::is_same_v<Generated3, gtk::IntegerSequence<int, 0, 1, 2>>);
    static_assert(std::is_same_v<Generated5, gtk::IntegerSequence<int, 0, 1, 2, 3, 4>>);

    // Test IndexSequence
    using IndexSeq0 = gtk::MakeIndexSequence<0>;
    using IndexSeq1 = gtk::MakeIndexSequence<1>;
    using IndexSeq2 = gtk::MakeIndexSequence<2>;
    using IndexSeq3 = gtk::MakeIndexSequence<3>;
    using IndexSeq5 = gtk::MakeIndexSequence<5>;

    static_assert(std::is_same_v<IndexSeq0, gtk::IntegerSequence<size_t>>);
    static_assert(std::is_same_v<IndexSeq1, gtk::IntegerSequence<size_t, 0>>);
    static_assert(std::is_same_v<IndexSeq2, gtk::IntegerSequence<size_t, 0, 1>>);
    static_assert(std::is_same_v<IndexSeq3, gtk::IntegerSequence<size_t, 0, 1, 2>>);
    static_assert(std::is_same_v<IndexSeq5, gtk::IntegerSequence<size_t, 0, 1, 2, 3, 4>>);
  }

  {
    // Filter Tests - IsEven
    using TestSeq = gtk::IntegerSequence<int, 1, 2, 3, 4, 5, 6>;
    using EvenFiltered = gtk::IntegerSequenceFilterT<TestSeq, gtk::IsEven>;
    using ExpectedEven = gtk::IntegerSequence<int, 2, 4, 6>;

    static_assert(std::is_same_v<EvenFiltered, ExpectedEven>);

    // Test filter with no matches
    using OddSeq = gtk::IntegerSequence<int, 1, 3, 5>;
    using EvenFromOdd = gtk::IntegerSequenceFilterT<OddSeq, gtk::IsEven>;
    using ExpectedEmpty = gtk::IntegerSequence<int>;

    static_assert(std::is_same_v<EvenFromOdd, ExpectedEmpty>);
  }

  {
    // Filter Tests - IsOdd
    using TestSeq = gtk::IntegerSequence<int, 1, 2, 3, 4, 5, 6>;
    using OddFiltered = gtk::IntegerSequenceFilterT<TestSeq, gtk::IsOdd>;
    using ExpectedOdd = gtk::IntegerSequence<int, 1, 3, 5>;

    static_assert(std::is_same_v<OddFiltered, ExpectedOdd>);

    // Test filter with all matches
    using AllOddSeq = gtk::IntegerSequence<int, 1, 3, 5, 7>;
    using AllOddFiltered = gtk::IntegerSequenceFilterT<AllOddSeq, gtk::IsOdd>;

    static_assert(std::is_same_v<AllOddFiltered, AllOddSeq>);
  }

  {
    // Filter Tests - Custom Predicates
    using TestSeq = gtk::IntegerSequence<int, 1, 2, 3, 4, 5>;
    
    // Test with built-in predicates only
    using EvenFiltered = gtk::IntegerSequenceFilterT<TestSeq, gtk::IsEven>;
    using ExpectedEven = gtk::IntegerSequence<int, 2, 4>;
    static_assert(std::is_same_v<EvenFiltered, ExpectedEven>);

    using OddFiltered = gtk::IntegerSequenceFilterT<TestSeq, gtk::IsOdd>;
    using ExpectedOdd = gtk::IntegerSequence<int, 1, 3, 5>;
    static_assert(std::is_same_v<OddFiltered, ExpectedOdd>);
  }

  {
    // Complex Combinations Tests
    using TestSeq = gtk::IntegerSequence<int, 1, 2, 3, 4, 5>;
    
    // Test push front then reverse
    using PushedSeq = gtk::IntegerSequencePushFrontT<TestSeq, int, 0>;
    using ReversedPushed = gtk::IntegerSequenceReverseT<PushedSeq>;
    using ExpectedReversed = gtk::IntegerSequence<int, 5, 4, 3, 2, 1, 0>;

    static_assert(std::is_same_v<ReversedPushed, ExpectedReversed>);

    // Test pop front then filter
    using PoppedSeq = gtk::IntegerSequencePopFrontT<TestSeq>;
    using FilteredPopped = gtk::IntegerSequenceFilterT<PoppedSeq, gtk::IsEven>;
    using ExpectedFiltered = gtk::IntegerSequence<int, 2, 4>;

    static_assert(std::is_same_v<FilteredPopped, ExpectedFiltered>);
  }

  {
    // Edge Cases Tests
    using EmptySeq = gtk::IntegerSequence<int>;
    
    // Test operations on empty sequence
    using EmptyReversed = gtk::IntegerSequenceReverseT<EmptySeq>;
    using EmptyFiltered = gtk::IntegerSequenceFilterT<EmptySeq, gtk::IsEven>;

    static_assert(std::is_same_v<EmptyReversed, EmptySeq>);
    static_assert(std::is_same_v<EmptyFiltered, EmptySeq>);

    // Test with different types
    using SizeSeq = gtk::IntegerSequence<size_t, 10, 20, 30>;
    using SizeReversed = gtk::IntegerSequenceReverseT<SizeSeq>;
    using ExpectedSizeReversed = gtk::IntegerSequence<size_t, 30, 20, 10>;

    static_assert(std::is_same_v<SizeReversed, ExpectedSizeReversed>);
  }

  {
    // IntegerSequenceTransform Tests
    using TestSeq = gtk::IntegerSequence<int, 1, 2, 3, 4>;
    
    // Create function objects that can be default-constructed and called
    struct Double { 
      constexpr int operator()(int x) const { return x * 2; } 
    };
    struct AddOne { 
      constexpr int operator()(int x) const { return x + 1; } 
    };
    struct Square { 
      constexpr int operator()(int x) const { return x * x; } 
    };
    
    // Test transform with doubling
    using DoubledSeq = gtk::IntegerSequenceTransform<TestSeq, Double>::Type;
    using ExpectedDoubled = gtk::IntegerSequence<int, 2, 4, 6, 8>;
    static_assert(std::is_same_v<DoubledSeq, ExpectedDoubled>);
    
    // Test transform with add one
    using AddedSeq = gtk::IntegerSequenceTransform<TestSeq, AddOne>::Type;
    using ExpectedAdded = gtk::IntegerSequence<int, 2, 3, 4, 5>;
    static_assert(std::is_same_v<AddedSeq, ExpectedAdded>);
    
    // Test transform with squaring
    using SquaredSeq = gtk::IntegerSequenceTransform<TestSeq, Square>::Type;
    using ExpectedSquared = gtk::IntegerSequence<int, 1, 4, 9, 16>;
    static_assert(std::is_same_v<SquaredSeq, ExpectedSquared>);
    
    // Test transform on empty sequence
    using EmptySeq = gtk::IntegerSequence<int>;
    using TransformedEmpty = gtk::IntegerSequenceTransform<EmptySeq, Double>::Type;
    static_assert(std::is_same_v<TransformedEmpty, EmptySeq>);
    
    // Test transform with different types
    using SizeSeq = gtk::IntegerSequence<size_t, 10, 20, 30>;
    struct HalveSize { 
      constexpr size_t operator()(size_t x) const { return x / 2; } 
    };
    using HalvedSeq = gtk::IntegerSequenceTransform<SizeSeq, HalveSize>::Type;
    using ExpectedHalved = gtk::IntegerSequence<size_t, 5, 10, 15>;
    static_assert(std::is_same_v<HalvedSeq, ExpectedHalved>);
    
    // Test with negative numbers
    using NegativeSeq = gtk::IntegerSequence<int, -2, -1, 0, 1, 2>;
    struct AbsValue { 
      constexpr int operator()(int x) const { return x < 0 ? -x : x; } 
    };
    using AbsSeq = gtk::IntegerSequenceTransform<NegativeSeq, AbsValue>::Type;
    using ExpectedAbs = gtk::IntegerSequence<int, 2, 1, 0, 1, 2>;
    static_assert(std::is_same_v<AbsSeq, ExpectedAbs>);
  }

  {
    // IntegerSequenceGenerator Tests (Direct)
    using Gen1 = gtk::IntegerSequenceGenerator<int, 0, 4, 1>;  // 0,1,2,3,4
    using Expected1 = gtk::IntegerSequence<int, 0, 1, 2, 3, 4>;
    static_assert(std::is_same_v<Gen1, Expected1>);
    
    // Test with stride 2
    using Gen2 = gtk::IntegerSequenceGenerator<int, 0, 6, 2>;  // 0,2,4,6
    using Expected2 = gtk::IntegerSequence<int, 0, 2, 4, 6>;
    static_assert(std::is_same_v<Gen2, Expected2>);
    
    // Test with stride 3
    using Gen3 = gtk::IntegerSequenceGenerator<int, 1, 10, 3>;  // 1,4,7,10
    using Expected3 = gtk::IntegerSequence<int, 1, 4, 7, 10>;
    static_assert(std::is_same_v<Gen3, Expected3>);
    
    // Test negative stride (decreasing)
    using Gen4 = gtk::IntegerSequenceGenerator<int, 10, 5, -1>;  // 10,9,8,7,6,5
    using Expected4 = gtk::IntegerSequence<int, 10, 9, 8, 7, 6, 5>;
    static_assert(std::is_same_v<Gen4, Expected4>);
    
    // Test negative stride with step 2
    using Gen5 = gtk::IntegerSequenceGenerator<int, 10, 0, -2>;  // 10,8,6,4,2,0
    using Expected5 = gtk::IntegerSequence<int, 10, 8, 6, 4, 2, 0>;
    static_assert(std::is_same_v<Gen5, Expected5>);
    
    // Test single element (current == target)
    using Gen6 = gtk::IntegerSequenceGenerator<int, 5, 5, 1>;  // 5
    using Expected6 = gtk::IntegerSequence<int, 5>;
    static_assert(std::is_same_v<Gen6, Expected6>);
    
    // Test with size_t
    using GenSize = gtk::IntegerSequenceGenerator<size_t, 0, 3, 1>;  // 0,1,2,3
    using ExpectedSize = gtk::IntegerSequence<size_t, 0, 1, 2, 3>;
    static_assert(std::is_same_v<GenSize, ExpectedSize>);
  }

  {
    // IntegerSequenceTraits Tests
    // Test increasing sequence traits
    static_assert(gtk::IntegerSequenceTraits<int, 0, 5, 1>::isIncreasing);
    static_assert(!gtk::IntegerSequenceTraits<int, 0, 5, 1>::isLast);  // 0 to 5, step 1: not last
    static_assert(!gtk::IntegerSequenceTraits<int, 4, 5, 1>::isLast);  // 4 to 5, step 1: not last (next would be 5)
    static_assert(gtk::IntegerSequenceTraits<int, 5, 5, 1>::isLast);   // 5 to 5, step 1: is last
    static_assert(gtk::IntegerSequenceTraits<int, 6, 5, 1>::isLast);   // 6 to 5, step 1: is last (overshot)
    
    // Test decreasing sequence traits
    static_assert(!gtk::IntegerSequenceTraits<int, 10, 5, -1>::isIncreasing);
    static_assert(!gtk::IntegerSequenceTraits<int, 10, 5, -1>::isLast);  // 10 to 5, step -1: not last
    static_assert(!gtk::IntegerSequenceTraits<int, 6, 5, -1>::isLast);   // 6 to 5, step -1: not last (next would be 5)
    static_assert(gtk::IntegerSequenceTraits<int, 5, 5, -1>::isLast);    // 5 to 5, step -1: is last
    static_assert(gtk::IntegerSequenceTraits<int, 4, 5, -1>::isLast);    // 4 to 5, step -1: is last (overshot)
    
    // Test with larger strides
    static_assert(!gtk::IntegerSequenceTraits<int, 0, 10, 3>::isLast);  // 0 to 10, step 3: not last (next: 3, 6, 9)
    static_assert(gtk::IntegerSequenceTraits<int, 9, 10, 3>::isLast);   // 9 to 10, step 3: is last (next would be 12, overshoots)
    static_assert(gtk::IntegerSequenceTraits<int, 10, 10, 3>::isLast);  // 10 to 10, step 3: is last
    static_assert(gtk::IntegerSequenceTraits<int, 12, 10, 3>::isLast);  // 12 to 10, step 3: is last (overshot)
    
    static_assert(!gtk::IntegerSequenceTraits<int, 20, 0, -5>::isLast); // 20 to 0, step -5: not last
    static_assert(gtk::IntegerSequenceTraits<int, 0, 0, -5>::isLast);   // 0 to 0, step -5: is last
    static_assert(gtk::IntegerSequenceTraits<int, -1, 0, -5>::isLast);  // -1 to 0, step -5: is last (overshot)
    
    // Test with size_t
    static_assert(gtk::IntegerSequenceTraits<size_t, 0, 5, 1>::isIncreasing);
    static_assert(!gtk::IntegerSequenceTraits<size_t, 0, 5, 1>::isLast);
    static_assert(!gtk::IntegerSequenceTraits<size_t, 4, 5, 1>::isLast);
    static_assert(gtk::IntegerSequenceTraits<size_t, 5, 5, 1>::isLast);
  }

  {
    // Compare Template Tests
    using Compare3 = gtk::CompareWith<int, 3>;
    using Compare5 = gtk::CompareWith<int, 5>;
    using CompareSizeT10 = gtk::CompareWith<size_t, 10>;
    
    // Test Equal
    static_assert(Compare3::Equal<3>::value);
    static_assert(!Compare3::Equal<5>::value);
    static_assert(Compare5::Equal<5>::value);
    static_assert(!Compare5::Equal<3>::value);
    static_assert(CompareSizeT10::Equal<10>::value);
    static_assert(!CompareSizeT10::Equal<20>::value);
    
    // Test NotEqual
    static_assert(!Compare3::NotEqual<3>::value);
    static_assert(Compare3::NotEqual<5>::value);
    static_assert(!Compare5::NotEqual<5>::value);
    static_assert(Compare5::NotEqual<3>::value);
    static_assert(!CompareSizeT10::NotEqual<10>::value);
    static_assert(CompareSizeT10::NotEqual<20>::value);
    
    // Test LessThan
    static_assert(!Compare3::LessThan<2>::value);  // 3 < 2: false
    static_assert(!Compare3::LessThan<3>::value);  // 3 < 3: false
    static_assert(Compare3::LessThan<4>::value);   // 3 < 4: true
    static_assert(Compare5::LessThan<10>::value);  // 5 < 10: true
    static_assert(!Compare5::LessThan<2>::value);  // 5 < 2: false
    
    // Test GreaterThan
    static_assert(Compare3::GreaterThan<2>::value);  // 3 > 2: true
    static_assert(!Compare3::GreaterThan<3>::value); // 3 > 3: false
    static_assert(!Compare3::GreaterThan<4>::value); // 3 > 4: false
    static_assert(!Compare5::GreaterThan<10>::value);// 5 > 10: false
    static_assert(Compare5::GreaterThan<2>::value);  // 5 > 2: true
    
    // Test LessThanOrEqual
    static_assert(!Compare3::LessThanOrEqual<2>::value); // 3 <= 2: false
    static_assert(Compare3::LessThanOrEqual<3>::value);  // 3 <= 3: true
    static_assert(Compare3::LessThanOrEqual<4>::value);  // 3 <= 4: true
    static_assert(Compare5::LessThanOrEqual<5>::value);  // 5 <= 5: true
    static_assert(Compare5::LessThanOrEqual<10>::value); // 5 <= 10: true
    static_assert(!Compare5::LessThanOrEqual<2>::value); // 5 <= 2: false
    
    // Test GreaterThanOrEqual
    static_assert(Compare3::GreaterThanOrEqual<2>::value);  // 3 >= 2: true
    static_assert(Compare3::GreaterThanOrEqual<3>::value);  // 3 >= 3: true
    static_assert(!Compare3::GreaterThanOrEqual<4>::value); // 3 >= 4: false
    static_assert(!Compare5::GreaterThanOrEqual<10>::value);// 5 >= 10: false
    static_assert(Compare5::GreaterThanOrEqual<5>::value);  // 5 >= 5: true
    static_assert(Compare5::GreaterThanOrEqual<2>::value);  // 5 >= 2: true
  }

  {
    // Advanced Generator Edge Cases
    // Test boundary conditions and edge cases for IntegerSequenceGenerator
    
    // Test where start > end with positive stride (should be single element)
    using EdgeGen1 = gtk::IntegerSequenceGenerator<int, 5, 3, 1>;
    // This should generate just [5] since the traits will mark it as last immediately
    using ExpectedEdge1 = gtk::IntegerSequence<int, 5>;
    static_assert(std::is_same_v<EdgeGen1, ExpectedEdge1>);
    
    // Test where start < end with negative stride (should be single element)
    using EdgeGen2 = gtk::IntegerSequenceGenerator<int, 3, 5, -1>;
    // This should generate just [3] since the traits will mark it as last immediately
    using ExpectedEdge2 = gtk::IntegerSequence<int, 3>;
    static_assert(std::is_same_v<EdgeGen2, ExpectedEdge2>);
    
    // Test with large stride that overshoots
    using EdgeGen3 = gtk::IntegerSequenceGenerator<int, 0, 2, 5>;
    // Should generate just [0] since next would be 5 which overshoots 2
    using ExpectedEdge3 = gtk::IntegerSequence<int, 0>;
    static_assert(std::is_same_v<EdgeGen3, ExpectedEdge3>);
    
    // Test negative numbers
    using EdgeGen4 = gtk::IntegerSequenceGenerator<int, -3, 2, 2>;  // -3, -1, 1
    using ExpectedEdge4 = gtk::IntegerSequence<int, -3, -1, 1>;
    static_assert(std::is_same_v<EdgeGen4, ExpectedEdge4>);
    
    // Test negative to negative
    using EdgeGen5 = gtk::IntegerSequenceGenerator<int, -5, -1, 1>;  // -5, -4, -3, -2, -1
    using ExpectedEdge5 = gtk::IntegerSequence<int, -5, -4, -3, -2, -1>;
    static_assert(std::is_same_v<EdgeGen5, ExpectedEdge5>);
    
    // Test that exact target is included
    using EdgeGen6 = gtk::IntegerSequenceGenerator<int, 0, 5, 1>;  // 0, 1, 2, 3, 4, 5
    using ExpectedEdge6 = gtk::IntegerSequence<int, 0, 1, 2, 3, 4, 5>;
    static_assert(std::is_same_v<EdgeGen6, ExpectedEdge6>);
    
    // Test reverse with exact target
    using EdgeGen7 = gtk::IntegerSequenceGenerator<int, 5, 0, -1>;  // 5, 4, 3, 2, 1, 0
    using ExpectedEdge7 = gtk::IntegerSequence<int, 5, 4, 3, 2, 1, 0>;
    static_assert(std::is_same_v<EdgeGen7, ExpectedEdge7>);
  }

  {
    // MakeIntegerSequence Edge Cases and Extended Tests
    // Test larger sequences
    using Large10 = gtk::MakeIntegerSequence<int, 10>::Type;
    using ExpectedLarge10 = gtk::IntegerSequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>;
    static_assert(std::is_same_v<Large10, ExpectedLarge10>);
    
    // Test with different integral types
    using CharSeq = gtk::MakeIntegerSequence<char, 3>::Type;
    using ExpectedChar = gtk::IntegerSequence<char, 0, 1, 2>;
    static_assert(std::is_same_v<CharSeq, ExpectedChar>);
    
    using ShortSeq = gtk::MakeIntegerSequence<short, 4>::Type;
    using ExpectedShort = gtk::IntegerSequence<short, 0, 1, 2, 3>;
    static_assert(std::is_same_v<ShortSeq, ExpectedShort>);
    
    // Test MakeIndexSequence with larger counts
    using LargeIndex = gtk::MakeIndexSequence<8>;
    using ExpectedLargeIndex = gtk::IntegerSequence<size_t, 0, 1, 2, 3, 4, 5, 6, 7>;
    static_assert(std::is_same_v<LargeIndex, ExpectedLargeIndex>);
  }
}