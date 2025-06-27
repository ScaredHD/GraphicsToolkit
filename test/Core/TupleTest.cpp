#include <gtest/gtest.h>
#include <array>

#include "Tuple.h"

TEST(Core, Tuple)
{
  using namespace gtk;

  // Basic construction and element access
  {
    auto t = MakeTuple(1, 2.5, 3.14);
    EXPECT_EQ(Get<0>(t), 1);
    EXPECT_EQ(Get<1>(t), 2.5);
    EXPECT_NEAR(Get<2>(t), 3.14, 1e-6);
  }

  // Tuple with different types
  {
    Tuple<int, double, char> t{42, 3.14, 'A'};
    EXPECT_EQ(Get<0>(t), 42);
    EXPECT_EQ(Get<1>(t), 3.14);
    EXPECT_EQ(Get<2>(t), 'A');
  }

  // Empty tuple
  {
    Tuple<> empty{};
    // Test that empty tuple exists and can be constructed
    (void)empty; // Suppress unused variable warning
    EXPECT_TRUE(true); // Compilation success is the test
  }

  // Single element tuple
  {
    Tuple<std::string> single{"hello"};
    EXPECT_EQ(Get<0>(single), "hello");
  }

  // Tuple equality
  {
    auto t1 = MakeTuple(1, 2, 3);
    auto t2 = MakeTuple(1, 2, 3);
    auto t3 = MakeTuple(1, 2, 4);
    
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 == t3);
    EXPECT_FALSE(t1 != t2);
    EXPECT_TRUE(t1 != t3);
  }

  // Tuple with const elements
  {
    const auto t = MakeTuple(10, 20.5f, 'B');
    EXPECT_EQ(Get<0>(t), 10);
    EXPECT_EQ(Get<1>(t), 20.5f);
    EXPECT_EQ(Get<2>(t), 'B');
  }

  // Copy constructor
  {
    auto original = MakeTuple(100, 200.0, "test");
    auto copy = original;
    
    EXPECT_EQ(Get<0>(copy), 100);
    EXPECT_EQ(Get<1>(copy), 200.0);
    EXPECT_EQ(Get<2>(copy), "test");
    EXPECT_TRUE(original == copy);
  }

  // Assignment
  {
    auto t1 = MakeTuple(1, 2.0, 'x');
    auto t2 = MakeTuple(0, 0.0, 'a');
    
    t2 = t1;
    EXPECT_TRUE(t1 == t2);
  }

  // Nested tuples
  {
    auto inner = MakeTuple(1, 2);
    auto outer = MakeTuple(inner, 3.14);
    
    auto retrieved_inner = Get<0>(outer);
    EXPECT_EQ(Get<0>(retrieved_inner), 1);
    EXPECT_EQ(Get<1>(retrieved_inner), 2);
    EXPECT_NEAR(Get<1>(outer), 3.14, 1e-6);
  }

  // Tuple with arrays/containers
  {
    std::array<int, 3> arr{1, 2, 3};
    auto t = MakeTuple(arr, "string");
    
    auto retrieved_arr = Get<0>(t);
    EXPECT_EQ(retrieved_arr[0], 1);
    EXPECT_EQ(retrieved_arr[1], 2);
    EXPECT_EQ(retrieved_arr[2], 3);
    EXPECT_EQ(Get<1>(t), "string");
  }

  // Large tuple
  {
    auto large = MakeTuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    EXPECT_EQ(Get<0>(large), 1);
    EXPECT_EQ(Get<4>(large), 5);
    EXPECT_EQ(Get<9>(large), 10);
  }

  // Tuple with boolean values
  {
    auto bools = MakeTuple(true, false, true);
    EXPECT_TRUE(Get<0>(bools));
    EXPECT_FALSE(Get<1>(bools));
    EXPECT_TRUE(Get<2>(bools));
  }

  // Tuple with mixed numeric types
  {
    auto mixed = MakeTuple(static_cast<short>(1), 2, 3L, 4.0f, 5.0);
    EXPECT_EQ(Get<0>(mixed), 1);
    EXPECT_EQ(Get<1>(mixed), 2);
    EXPECT_EQ(Get<2>(mixed), 3L);
    EXPECT_EQ(Get<3>(mixed), 4.0f);
    EXPECT_EQ(Get<4>(mixed), 5.0);
  }

  // Type checking at compile time
  {
    auto t = MakeTuple(42, 3.14, 'Z');
    // Test that we can access elements of correct types
    int first = Get<0>(t);
    double second = Get<1>(t);
    char third = Get<2>(t);
    
    EXPECT_EQ(first, 42);
    EXPECT_NEAR(second, 3.14, 1e-6);
    EXPECT_EQ(third, 'Z');
  }

  // Tuple with pointers
  {
    int value = 42;
    int* ptr = &value;
    auto t = MakeTuple(ptr, nullptr);
    
    EXPECT_EQ(*Get<0>(t), 42);
    EXPECT_EQ(Get<1>(t), nullptr);
  }

  // Tuple construction from various sources
  {
    // Direct construction
    Tuple<int, float> direct{10, 3.5f};
    
    // MakeTuple construction
    auto made = MakeTuple(10, 3.5f);
    
    EXPECT_TRUE(direct == made);
  }

  // Tuple transform tests
  {
    // Basic unary transform - multiply each element by 2
    auto original = MakeTuple(1, 2, 3);
    auto doubled = Transform(original, [](int x) { return x * 2; });
    
    EXPECT_EQ(Get<0>(doubled), 2);
    EXPECT_EQ(Get<1>(doubled), 4);
    EXPECT_EQ(Get<2>(doubled), 6);
  }

  // Transform with type conversion
  {
    auto ints = MakeTuple(1, 2, 3, 4);
    auto strings = Transform(ints, [](int x) { return std::to_string(x); });
    
    EXPECT_EQ(Get<0>(strings), "1");
    EXPECT_EQ(Get<1>(strings), "2");
    EXPECT_EQ(Get<2>(strings), "3");
    EXPECT_EQ(Get<3>(strings), "4");
  }

  // Transform with mixed types
  {
    auto mixed = MakeTuple(1, 2.5f, 'A');
    auto transformed = Transform(mixed, [](auto x) { 
      if constexpr (std::is_same_v<decltype(x), int>) {
        return x + 10;
      } else if constexpr (std::is_same_v<decltype(x), float>) {
        return x * 2.0f;
      } else {
        return static_cast<char>(x + 1);
      }
    });
    
    EXPECT_EQ(Get<0>(transformed), 11);
    EXPECT_EQ(Get<1>(transformed), 5.0f);
    EXPECT_EQ(Get<2>(transformed), 'B');
  }

  // Transform empty tuple
  {
    Tuple<> empty{};
    auto transformed = Transform(empty, [](int x) { return x * 2; });
    // Should compile and work with empty tuple
    (void)transformed;
    EXPECT_TRUE(true);
  }

  // Binary transform - addition
  {
    auto t1 = MakeTuple(1, 2, 3);
    auto t2 = MakeTuple(10, 20, 30);
    auto sum = Transform(t1, t2, [](int a, int b) { return a + b; });
    
    EXPECT_EQ(Get<0>(sum), 11);
    EXPECT_EQ(Get<1>(sum), 22);
    EXPECT_EQ(Get<2>(sum), 33);
  }

  // Binary transform with different types
  {
    auto ints = MakeTuple(1, 2, 3);
    auto floats = MakeTuple(1.5f, 2.5f, 3.5f);
    auto products = Transform(ints, floats, [](int a, float b) { return static_cast<float>(a) * b; });
    
    EXPECT_EQ(Get<0>(products), 1.5f);
    EXPECT_EQ(Get<1>(products), 5.0f);
    EXPECT_EQ(Get<2>(products), 10.5f);
  }

  // Binary transform - string concatenation
  {
    auto strings1 = MakeTuple(std::string("Hello"), std::string("Good"), std::string("Nice"));
    auto strings2 = MakeTuple(std::string(" World"), std::string(" Day"), std::string(" Work"));
    auto concatenated = Transform(strings1, strings2, [](const std::string& a, const std::string& b) {
      return a + b;
    });
    
    EXPECT_EQ(Get<0>(concatenated), "Hello World");
    EXPECT_EQ(Get<1>(concatenated), "Good Day");
    EXPECT_EQ(Get<2>(concatenated), "Nice Work");
  }

  // Transform with function object
  {
    auto square = [](auto x) { return x * x; };
    auto values = MakeTuple(2, 3, 4);
    auto squared = Transform(values, square);
    
    EXPECT_EQ(Get<0>(squared), 4);
    EXPECT_EQ(Get<1>(squared), 9);
    EXPECT_EQ(Get<2>(squared), 16);
  }

  // Transform with capture lambda
  {
    int offset = 100;
    auto numbers = MakeTuple(1, 2, 3, 4);
    auto offset_numbers = Transform(numbers, [offset](int x) { return x + offset; });
    
    EXPECT_EQ(Get<0>(offset_numbers), 101);
    EXPECT_EQ(Get<1>(offset_numbers), 102);
    EXPECT_EQ(Get<2>(offset_numbers), 103);
    EXPECT_EQ(Get<3>(offset_numbers), 104);
  }

  // Chained transforms
  {
    auto original = MakeTuple(1, 2, 3);
    auto doubled = Transform(original, [](int x) { return x * 2; });
    auto doubled_plus_one = Transform(doubled, [](int x) { return x + 1; });
    
    EXPECT_EQ(Get<0>(doubled_plus_one), 3);  // (1*2)+1 = 3
    EXPECT_EQ(Get<1>(doubled_plus_one), 5);  // (2*2)+1 = 5
    EXPECT_EQ(Get<2>(doubled_plus_one), 7);  // (3*2)+1 = 7
  }

}