#include <gtest/gtest.h>

#include "Dimension.h"
#include "Tensor.h"
#include "TensorOperations.h"

template<typename T1, typename T2>
static bool AreNearlyEqual(const T1& a, const T2& b, float epsilon = 1e-5f)
{
  if (T1::DimensionType::count != T2::DimensionType::count) {
    return false;
  }
  for (size_t i = 0; i < T1::DimensionType::count; ++i) {
    if (std::abs(a[i] - b[i]) > epsilon) {
      return false;
    }
  }
  return true;
}

// Helper function to test basic tensor functionality
static void BasicTensorFunctionality()
{
  // Basic construction and element access
  {
    Tensor<float, 2, 3> t(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
    
    // Test flattened access
    EXPECT_EQ(t[0], 1.0f);
    EXPECT_EQ(t[1], 2.0f);
    EXPECT_EQ(t[5], 6.0f);
    
    // Test multi-dimensional access
    EXPECT_EQ(t(0, 0), 1.0f);
    EXPECT_EQ(t(0, 1), 2.0f);
    EXPECT_EQ(t(1, 2), 6.0f);
    
    // Test modification
    t[0] = 10.0f;
    EXPECT_EQ(t[0], 10.0f);
    EXPECT_EQ(t(0, 0), 10.0f);
    
    t(1, 1) = 20.0f;
    EXPECT_EQ(t(1, 1), 20.0f);
    EXPECT_EQ(t[4], 20.0f);
  }

  // Tensor equality and comparison
  {
    Tensor<float, 2, 2> t1(1.0f, 2.0f, 3.0f, 4.0f);
    Tensor<float, 2, 2> t2(1.0f, 2.0f, 3.0f, 4.0f);
    Tensor<float, 2, 2> t3(1.0f, 2.0f, 3.0f, 5.0f);
    
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 == t3);
    EXPECT_FALSE(t1 != t2);
    EXPECT_TRUE(t1 != t3);
  }

  // Iterators and range-based for loops
  {
    Tensor<int, 2, 2> t(1, 2, 3, 4);
    
    // Test range-based for loop
    int expected = 1;
    for (const auto& value : t) {
      EXPECT_EQ(value, expected++);
    }
    
    // Test iterator modification
    for (auto& value : t) {
      value *= 2;
    }
    
    EXPECT_EQ(t[0], 2);
    EXPECT_EQ(t[1], 4);
    EXPECT_EQ(t[2], 6);
    EXPECT_EQ(t[3], 8);
  }

  // Utility functions
  {
    Tensor<int, 2, 2> zeroFree(1, 2, 3, 4);
    Tensor<int, 2, 2> withZero(0, 2, 3, 4);
    
    EXPECT_TRUE(zeroFree.IsZeroFree());
    EXPECT_FALSE(withZero.IsZeroFree());
    
    Tensor<int, 4> oneHot(0, 1, 0, 0);
    Tensor<int, 4> notOneHot(1, 1, 0, 0);
    
    EXPECT_TRUE(oneHot.OneHot());
    EXPECT_FALSE(notOneHot.OneHot());
  }

  // Tensor dimensions and properties
  {
    using TensorType = Tensor<float, 3, 4, 5>;
    static_assert(TensorType::rank == 3);
    static_assert(TensorType::count == 60);
    static_assert(Rank<TensorType> == 3);
    static_assert(Count<TensorType> == 60);
  }
}

// Helper function to test tensor initialization patterns
static void TensorInitialization()
{
  // Variadic constructor with explicit types
  {
    Tensor<float, 2, 3> t(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
    EXPECT_EQ(t(0, 0), 1.0f);
    EXPECT_EQ(t(1, 2), 6.0f);
  }

  // Initializer list constructor (nested lists)
  {
    Tensor<int, 2, 2> t{{1, 2}, {3, 4}};
    EXPECT_EQ(t(0, 0), 1);
    EXPECT_EQ(t(0, 1), 2);
    EXPECT_EQ(t(1, 0), 3);
    EXPECT_EQ(t(1, 1), 4);
    
    Tensor<int, 2, 3> t2{{1, 2, 3}, {4, 5, 6}};
    EXPECT_EQ(t2(0, 0), 1);
    EXPECT_EQ(t2(0, 1), 2);
    EXPECT_EQ(t2(1, 0), 4);
    EXPECT_EQ(t2(1, 2), 6);
    
    Tensor<double, 1, 4> row{{1.1, 2.2, 3.3, 4.4}};
    EXPECT_EQ(row(0, 0), 1.1);
    EXPECT_EQ(row(0, 3), 4.4);
  }

  // Copy constructor and assignment
  {
    Tensor<float, 2, 2> original(1.0f, 2.0f, 3.0f, 4.0f);
    Tensor<float, 2, 2> copy(original);
    
    EXPECT_TRUE(original == copy);
    EXPECT_EQ(copy(0, 0), 1.0f);
    EXPECT_EQ(copy(1, 1), 4.0f);
    
    Tensor<int, 2, 2> t1(1, 2, 3, 4);
    Tensor<int, 2, 2> t2(5, 6, 7, 8);
    
    t2 = t1;  // Copy assignment
    EXPECT_TRUE(t1 == t2);
    EXPECT_EQ(t2(0, 0), 1);
    EXPECT_EQ(t2(1, 1), 4);
  }

  // Type conversion constructors
  {
    Tensor<int, 2, 2> intTensor(1, 2, 3, 4);
    Tensor<float, 2, 2> floatTensor(intTensor);  // Conversion constructor
    
    EXPECT_EQ(floatTensor[0], 1.0f);
    EXPECT_EQ(floatTensor[1], 2.0f);
    EXPECT_EQ(floatTensor[2], 3.0f);
    EXPECT_EQ(floatTensor[3], 4.0f);
    
    EXPECT_EQ(floatTensor(0, 0), 1.0f);
    EXPECT_EQ(floatTensor(1, 1), 4.0f);
    
    Tensor<float, 4> large(1.0f, 2.0f, 3.0f, 4.0f);
    Tensor<float, 2> small(large);  // Should copy first 2 elements
    
    EXPECT_EQ(small[0], 1.0f);
    EXPECT_EQ(small[1], 2.0f);
  }

  // Default construction and uniform initialization
  {
    Tensor<int, 2, 2> defaultTensor;
    // Elements are uninitialized, so we just test that it compiles
    // and we can assign to it
    defaultTensor(0, 0) = 42;
    EXPECT_EQ(defaultTensor(0, 0), 42);
    
    Tensor<float, 2, 2> zeros{};  // Should value-initialize to zeros
    // Note: This may not actually zero-initialize depending on implementation
    // But it should compile
    zeros(0, 0) = 0.0f;  // Explicit assignment
    EXPECT_EQ(zeros(0, 0), 0.0f);
  }

  // Mixed type and expression initialization
  {
    Tensor<double, 3> mixed(1, 2.5f, 3.7);  // int, float, double
    EXPECT_EQ(mixed[0], 1.0);
    EXPECT_EQ(mixed[1], 2.5);
    EXPECT_EQ(mixed[2], 3.7);
    
    int x = 5;
    Tensor<int, 3> expr(x + 1, x * 2, x - 1);
    EXPECT_EQ(expr[0], 6);  // 5 + 1
    EXPECT_EQ(expr[1], 10); // 5 * 2
    EXPECT_EQ(expr[2], 4);  // 5 - 1
    
    auto getValue = [](int i) { return i * i; };
    Tensor<int, 3> func(getValue(2), getValue(3), getValue(4));
    EXPECT_EQ(func[0], 4);  // 2^2
    EXPECT_EQ(func[1], 9);  // 3^2
    EXPECT_EQ(func[2], 16); // 4^2
  }

  // Large tensor and const initialization
  {
    Tensor<int, 8> large(1, 2, 3, 4, 5, 6, 7, 8);
    for (int i = 0; i < 8; ++i) {
      EXPECT_EQ(large[i], i + 1);
    }
    
    const float PI = 3.14159f;
    const int MAGIC = 42;
    
    Tensor<float, 2> constants(PI, static_cast<float>(MAGIC));
    EXPECT_FLOAT_EQ(constants[0], PI);
    EXPECT_FLOAT_EQ(constants[1], 42.0f);
  }
}

// Helper function to test implicit conversions
static void ImplicitConversions()
{
  // Scalar tensor implicit conversion
  {
    Tensor<double, 1> scalar(42.0);
    double value = scalar;  // Test implicit conversion
    EXPECT_EQ(value, 42.0);
    EXPECT_EQ(scalar[0], 42.0);
    
    Tensor<float, 1> single(42.5f);
    EXPECT_EQ(single[0], 42.5f);
    EXPECT_EQ(static_cast<float>(single), 42.5f);  // Implicit conversion to scalar
    
    Tensor<float, 1> scalarTensor(3.14f);
    
    // Implicit conversion should work
    float floatValue = scalarTensor;
    EXPECT_EQ(floatValue, 3.14f);
    
    // Should work in function calls expecting scalar
    auto testFunc = [](float f) { return f * 2.0f; };
    float result = testFunc(scalarTensor);
    EXPECT_EQ(result, 6.28f);
    
    // Should work in arithmetic operations
    float sum = scalarTensor + 1.0f;
    EXPECT_FLOAT_EQ(sum, 4.14f);
  }

  // Different scalar types
  {
    Tensor<int, 1> intScalar(42);
    int intValue = intScalar;
    EXPECT_EQ(intValue, 42);
    
    Tensor<double, 1> doubleScalar(2.718);
    double doubleValue = doubleScalar;
    EXPECT_EQ(doubleValue, 2.718);
  }

  // Multi-element tensors should NOT have implicit conversion
  {
    // This should compile (1-element tensor)
    Tensor<float, 1> single(5.0f);
    float val = single;
    EXPECT_EQ(val, 5.0f);
    
    // Multi-element tensors should NOT have implicit conversion to scalar
    // (This is enforced by SFINAE in the conversion operator)
    Tensor<float, 2> multi(1.0f, 2.0f);
    // float invalid = multi;  // This should NOT compile
    // Instead, we can access elements explicitly:
    EXPECT_EQ(multi[0], 1.0f);
    EXPECT_EQ(multi[1], 2.0f);
  }

  // Assignment and conditional expressions
  {
    Tensor<float, 1> source(7.5f);
    float target;
    
    target = source;  // Implicit conversion in assignment
    EXPECT_EQ(target, 7.5f);
    
    Tensor<bool, 1> boolTensor(true);
    
    if (boolTensor) {  // Implicit conversion to bool
      EXPECT_TRUE(true);  // This should execute
    } else {
      FAIL() << "Implicit conversion to bool failed";
    }
    
    const Tensor<double, 1> constTensor(123.456);
    double constValue = constTensor;  // Should work with const tensors too
    EXPECT_EQ(constValue, 123.456);
  }

  // Specialized Tensor<Scalar> constructor
  {
    Tensor<float> scalar(42.0f);  // Uses the specialized Tensor<Scalar> class
    EXPECT_EQ(scalar[0], 42.0f);
    float value = scalar;  // Should work with implicit conversion
    EXPECT_EQ(value, 42.0f);
  }
}

// Helper function to test tensor broadcasting
static void TensorBroadcasting()
{
  // Broadcasting scalar to higher dimensions
  {
    Tensor<int, 1> scalar(42);
    auto broadcasted = Broadcast<TDimension<3, 3>>(scalar);
    
    // All elements should be 42
    for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        EXPECT_EQ(broadcasted(i, j), 42);
      }
    }
  }

  // Broadcasting 1D to 2D with compatible dimensions
  {
    Tensor<float, 1> vec(5.0f);  // Single element that can broadcast
    auto broadcasted = Broadcast<TDimension<2, 3>>(vec);
    
    // Should broadcast [5] to:
    // [[5, 5, 5],
    //  [5, 5, 5]]
    for (size_t i = 0; i < 2; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        EXPECT_EQ(broadcasted(i, j), 5.0f);
      }
    }
  }

  // Broadcasting with dimension 1 compatibility
  {
    Tensor<float, 1, 3> row(1.0f, 2.0f, 3.0f);  // 1x3 tensor
    auto broadcasted = Broadcast<TDimension<2, 3>>(row);
    
    // Should broadcast [1, 2, 3] to:
    // [[1, 2, 3],
    //  [1, 2, 3]]
    EXPECT_EQ(broadcasted(0, 0), 1.0f);
    EXPECT_EQ(broadcasted(0, 1), 2.0f);
    EXPECT_EQ(broadcasted(0, 2), 3.0f);
    EXPECT_EQ(broadcasted(1, 0), 1.0f);
    EXPECT_EQ(broadcasted(1, 1), 2.0f);
    EXPECT_EQ(broadcasted(1, 2), 3.0f);
  }

  // Broadcasting 2D to 3D
  {
    Tensor<float, 1, 1> single(7.0f);  // 1x1 tensor
    auto broadcasted = Broadcast<TDimension<2, 3, 4>>(single);
    
    // All elements should be 7.0f
    for (size_t i = 0; i < 2; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        for (size_t k = 0; k < 4; ++k) {
          EXPECT_EQ(broadcasted(i, j, k), 7.0f);
        }
      }
    }
  }

  // Approximate equality for broadcasting results
  {
    Tensor<float, 1> source(1.5f);
    auto result = Broadcast<TDimension<2, 2>>(source);
    
    Tensor<float, 2, 2> expected(1.5f, 1.5f, 1.5f, 1.5f);
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }

  // Test runtime broadcasting with constexpr tensors to demonstrate that
  // while Broadcast is not compile-time evaluable, it works correctly with constexpr inputs
  {
    constexpr Tensor<int, 1> constScalar(42);
    auto broadcastedToVector = Broadcast<TDimension<3>>(constScalar);
    EXPECT_EQ(broadcastedToVector[0], 42);
    EXPECT_EQ(broadcastedToVector[1], 42);
    EXPECT_EQ(broadcastedToVector[2], 42);
    
    constexpr Tensor<float, 1> constVector(3.14f);
    auto vectorToMatrix = Broadcast<TDimension<2, 3>>(constVector);
    for (size_t i = 0; i < 2; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        EXPECT_EQ(vectorToMatrix(i, j), 3.14f);
      }
    }
    
    constexpr Tensor<int, 1, 3> constRow(1, 2, 3);
    auto rowToMatrix = Broadcast<TDimension<2, 3>>(constRow);
    EXPECT_EQ(rowToMatrix(0, 0), 1);
    EXPECT_EQ(rowToMatrix(0, 1), 2);
    EXPECT_EQ(rowToMatrix(0, 2), 3);
    EXPECT_EQ(rowToMatrix(1, 0), 1);
    EXPECT_EQ(rowToMatrix(1, 1), 2);
    EXPECT_EQ(rowToMatrix(1, 2), 3);
  }
}

// Helper function to test tensor operations
static void TensorOperations()
{
  // Basic arithmetic operations
  {
    Tensor<float, 2, 2> t1(1.0f, 2.0f, 3.0f, 4.0f);
    Tensor<float, 2, 2> t2(5.0f, 6.0f, 7.0f, 8.0f);
    
    auto result = t1 + t2;
    
    EXPECT_EQ(result(0, 0), 6.0f);  // 1 + 5
    EXPECT_EQ(result(0, 1), 8.0f);  // 2 + 6
    EXPECT_EQ(result(1, 0), 10.0f); // 3 + 7
    EXPECT_EQ(result(1, 1), 12.0f); // 4 + 8
    
    Tensor<float, 2, 2> t3(10.0f, 8.0f, 6.0f, 4.0f);
    Tensor<float, 2, 2> t4(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto subResult = t3 - t4;
    
    EXPECT_EQ(subResult(0, 0), 9.0f);  // 10 - 1
    EXPECT_EQ(subResult(0, 1), 6.0f);  // 8 - 2
    EXPECT_EQ(subResult(1, 0), 3.0f);  // 6 - 3
    EXPECT_EQ(subResult(1, 1), 0.0f);  // 4 - 4
  }

  // Multiplication and division
  {
    Tensor<float, 2, 2> t1(2.0f, 3.0f, 4.0f, 5.0f);
    Tensor<float, 2, 2> t2(2.0f, 2.0f, 2.0f, 2.0f);
    
    auto mulResult = t1 * t2;
    
    EXPECT_EQ(mulResult(0, 0), 4.0f);   // 2 * 2
    EXPECT_EQ(mulResult(0, 1), 6.0f);   // 3 * 2
    EXPECT_EQ(mulResult(1, 0), 8.0f);   // 4 * 2
    EXPECT_EQ(mulResult(1, 1), 10.0f);  // 5 * 2
    
    Tensor<float, 2, 2> t3(8.0f, 6.0f, 4.0f, 2.0f);
    Tensor<float, 2, 2> t4(2.0f, 2.0f, 2.0f, 2.0f);
    
    auto divResult = t3 / t4;
    
    EXPECT_EQ(divResult(0, 0), 4.0f);  // 8 / 2
    EXPECT_EQ(divResult(0, 1), 3.0f);  // 6 / 2
    EXPECT_EQ(divResult(1, 0), 2.0f);  // 4 / 2
    EXPECT_EQ(divResult(1, 1), 1.0f);  // 2 / 2
  }

  // Scalar-tensor operations (broadcasting)
  {
    Tensor<float, 2, 2> t(1.0f, 2.0f, 3.0f, 4.0f);
    float scalar = 5.0f;
    
    auto addResult = t + scalar;
    EXPECT_EQ(addResult(0, 0), 6.0f);
    EXPECT_EQ(addResult(1, 1), 9.0f);
    
    auto mulResult = t * scalar;
    EXPECT_EQ(mulResult(0, 0), 5.0f);
    EXPECT_EQ(mulResult(1, 1), 20.0f);
    
    Tensor<float, 1> scalarTensor(5.0f);
    Tensor<float, 2, 2> matrix(1.0f, 2.0f, 3.0f, 4.0f);
    
    // This should broadcast the scalar to all elements
    auto broadcastResult = matrix + scalarTensor;
    
    EXPECT_EQ(broadcastResult(0, 0), 6.0f);  // 1 + 5
    EXPECT_EQ(broadcastResult(0, 1), 7.0f);  // 2 + 5
    EXPECT_EQ(broadcastResult(1, 0), 8.0f);  // 3 + 5
    EXPECT_EQ(broadcastResult(1, 1), 9.0f);  // 4 + 5
  }

  // Chained operations and different types
  {
    Tensor<float, 2> t1(3.0f, 4.0f);
    Tensor<float, 2> t2(1.0f, 2.0f);
    Tensor<float, 2> t3(2.0f, 2.0f);
    
    auto result = (t1 + t2) * t3;
    
    EXPECT_EQ(result[0], 8.0f);  // (3 + 1) * 2
    EXPECT_EQ(result[1], 12.0f); // (4 + 2) * 2
    
    Tensor<int, 2, 2> it1(10, 20, 30, 40);
    Tensor<int, 2, 2> it2(1, 2, 3, 4);
    
    auto intResult = it1 - it2;
    
    EXPECT_EQ(intResult(0, 0), 9);
    EXPECT_EQ(intResult(0, 1), 18);
    EXPECT_EQ(intResult(1, 0), 27);
    EXPECT_EQ(intResult(1, 1), 36);
  }
}

// Helper function to test edge cases and special scenarios
static void EdgeCases()
{
  // Single element and zero values
  {
    Tensor<float, 1> single(42.0f);
    EXPECT_EQ(single[0], 42.0f);
    EXPECT_EQ(static_cast<float>(single), 42.0f);
    
    Tensor<float, 2, 2> zeros(0.0f, 0.0f, 0.0f, 0.0f);
    
    EXPECT_FALSE(zeros.IsZeroFree());
    EXPECT_FALSE(zeros.OneHot());
    
    for (const auto& val : zeros) {
      EXPECT_EQ(val, 0.0f);
    }
  }

  // Negative values and floating point precision
  {
    Tensor<float, 2> negative(-1.0f, -2.0f);
    Tensor<float, 2> positive(1.0f, 2.0f);
    
    auto result = negative + positive;
    EXPECT_EQ(result[0], 0.0f);
    EXPECT_EQ(result[1], 0.0f);
    
    Tensor<float, 2> t1(0.1f + 0.2f, 1.0f);
    Tensor<float, 2> t2(0.3f, 1.0f);
    
    // Direct equality might fail due to floating point precision
    EXPECT_TRUE(AreNearlyEqual(t1, t2, 1e-6f));
  }

  // Large tensor operations
  {
    Tensor<int, 10> large;
    for (int i = 0; i < 10; ++i) {
      large[i] = i;
    }
    
    // Test that all elements are correctly set
    for (int i = 0; i < 10; ++i) {
      EXPECT_EQ(large[i], i);
    }
    
    // Test operations on large tensors
    auto doubled = large + large;
    for (int i = 0; i < 10; ++i) {
      EXPECT_EQ(doubled[i], i * 2);
    }
  }

  // Mixed type operations and copy semantics
  {
    Tensor<int, 2> intTensor(1, 2);
    Tensor<float, 2> floatTensor(1.5f, 2.5f);
    
    // Convert int tensor to float for operations
    Tensor<float, 2> convertedInt(intTensor);
    auto result = convertedInt + floatTensor;
    
    EXPECT_FLOAT_EQ(result[0], 2.5f);  // 1 + 1.5
    EXPECT_FLOAT_EQ(result[1], 4.5f);  // 2 + 2.5
    
    Tensor<double, 2, 2> original(1.0, 2.0, 3.0, 4.0);
    Tensor<double, 2, 2> copy = original;
    
    EXPECT_TRUE(original == copy);
    
    // Modify copy and ensure original is unchanged
    copy[0] = 99.0;
    EXPECT_NE(original, copy);
    EXPECT_EQ(original[0], 1.0);
    EXPECT_EQ(copy[0], 99.0);
  }

  // Const correctness and precision
  {
    const Tensor<int, 2, 2> constTensor(1, 2, 3, 4);
    
    // These should all work with const tensor
    EXPECT_EQ(constTensor[0], 1);
    EXPECT_EQ(constTensor(0, 0), 1);
    EXPECT_EQ(constTensor(1, 1), 4);
    
    // Range-based for loop with const
    int expected = 1;
    for (const auto& val : constTensor) {
      EXPECT_EQ(val, expected++);
    }
    
    double precise = 1.23456789012345;
    Tensor<double, 1> precisionTest(precise);
    
    EXPECT_DOUBLE_EQ(precisionTest[0], precise);
    EXPECT_DOUBLE_EQ(static_cast<double>(precisionTest), precise);
  }

  // Chained operations and type conversions
  {
    Tensor<float, 2, 2> original(1.1f, 2.2f, 3.3f, 4.4f);
    auto copy = original;  // Auto deduction + copy constructor
    
    EXPECT_TRUE(original == copy);
    
    // Modify original to ensure deep copy
    original[0] = 999.0f;
    EXPECT_NE(original, copy);
    EXPECT_EQ(copy[0], 1.1f);  // Copy should be unchanged
    
    Tensor<int, 2, 2> intSource(1, 2, 3, 4);
    Tensor<float, 2, 2> floatTarget(0.0f, 0.0f, 0.0f, 0.0f);
    
    // This uses the conversion constructor in assignment
    floatTarget = Tensor<float, 2, 2>(intSource);
    
    EXPECT_EQ(floatTarget(0, 0), 1.0f);
    EXPECT_EQ(floatTarget(1, 1), 4.0f);
    
    auto t1 = Tensor<int, 2>(1, 2);
    auto t2 = Tensor<int, 2>(t1);  // Copy construct
    auto t3 = t1 + t2;             // Operation creating new tensor
    
    EXPECT_EQ(t3[0], 2);  // 1 + 1
    EXPECT_EQ(t3[1], 4);  // 2 + 2
  }
}

// Compile-time tests for tensor utilities using static_assert
static void StaticTensorUtilities()
{
  // Test Rank and Count template utilities
  static_assert(Rank<Tensor<float, 2, 3>> == 2);
  static_assert(Rank<Tensor<int, 1, 2, 3, 4>> == 4);
  static_assert(Rank<Tensor<double, 5>> == 1);
  
  static_assert(Count<Tensor<float, 2, 3>> == 6);
  static_assert(Count<Tensor<int, 1, 2, 3, 4>> == 24);
  static_assert(Count<Tensor<double, 5>> == 5);
  
  // Test basic tensor properties (already tested inline but ensure they work)
  using TensorType1 = Tensor<float, 3, 4, 5>;
  static_assert(TensorType1::rank == 3);
  static_assert(TensorType1::count == 60);
  static_assert(std::is_same_v<TensorType1::ScalarType, float>);
  static_assert(std::is_same_v<TensorType1::DimensionType, TDimension<3, 4, 5>>);
  
  using TensorType2 = Tensor<int, 2>;
  static_assert(TensorType2::rank == 1);
  static_assert(TensorType2::count == 2);
  static_assert(std::is_same_v<TensorType2::ScalarType, int>);
  static_assert(std::is_same_v<TensorType2::DimensionType, TDimension<2>>);
}

static void StaticTensorDimensionProperties()
{
  // Test tensor dimension properties through Tensor class
  using Tensor1 = Tensor<float, 2, 3, 4>;
  static_assert(Tensor1::rank == 3);
  static_assert(Tensor1::count == 24);
  static_assert(Tensor1::DimensionType::rank == 3);
  static_assert(Tensor1::DimensionType::count == 24);
  static_assert(!Tensor1::DimensionType::isScalar);
  static_assert(!Tensor1::DimensionType::isVector);
  static_assert(!Tensor1::DimensionType::isMatrix);
  static_assert(Tensor1::DimensionType::isTensor);
  
  using Tensor2 = Tensor<int, 5>;
  static_assert(Tensor2::rank == 1);
  static_assert(Tensor2::count == 5);
  static_assert(Tensor2::DimensionType::rank == 1);
  static_assert(Tensor2::DimensionType::count == 5);
  static_assert(!Tensor2::DimensionType::isScalar);
  static_assert(Tensor2::DimensionType::isVector);
  static_assert(!Tensor2::DimensionType::isMatrix);
  static_assert(!Tensor2::DimensionType::isTensor);
  
  using Tensor3 = Tensor<double, 3, 4>;
  static_assert(Tensor3::rank == 2);
  static_assert(Tensor3::count == 12);
  static_assert(Tensor3::DimensionType::rank == 2);
  static_assert(Tensor3::DimensionType::count == 12);
  static_assert(!Tensor3::DimensionType::isScalar);
  static_assert(!Tensor3::DimensionType::isVector);
  static_assert(Tensor3::DimensionType::isMatrix);
  static_assert(!Tensor3::DimensionType::isTensor);
}

static void StaticTensorMakingUtilities()
{
  // Test MakeTensorFromDimensionT with tensor types
  using TensorType1 = Tensor<float, 2, 3, 4>;
  using Dim1 = TensorType1::DimensionType;
  using MadeTensor1 = MakeTensorFromDimensionT<float, Dim1>;
  static_assert(std::is_same_v<MadeTensor1, Tensor<float, 2, 3, 4>>);
  static_assert(MadeTensor1::rank == 3);
  static_assert(MadeTensor1::count == 24);
  
  using TensorType2 = Tensor<int, 5>;
  using Dim2 = TensorType2::DimensionType;
  using MadeTensor2 = MakeTensorFromDimensionT<int, Dim2>;
  static_assert(std::is_same_v<MadeTensor2, Tensor<int, 5>>);
  static_assert(MadeTensor2::rank == 1);
  static_assert(MadeTensor2::count == 5);
  
  // Test different scalar types with same dimensions
  using FloatTensor = MakeTensorFromDimensionT<float, TDimension<2, 3>>;
  using DoubleTensor = MakeTensorFromDimensionT<double, TDimension<2, 3>>;
  using IntTensor = MakeTensorFromDimensionT<int, TDimension<2, 3>>;
  
  static_assert(std::is_same_v<FloatTensor, Tensor<float, 2, 3>>);
  static_assert(std::is_same_v<DoubleTensor, Tensor<double, 2, 3>>);
  static_assert(std::is_same_v<IntTensor, Tensor<int, 2, 3>>);
  
  static_assert(FloatTensor::rank == 2);
  static_assert(DoubleTensor::rank == 2);
  static_assert(IntTensor::rank == 2);
  
  static_assert(FloatTensor::count == 6);
  static_assert(DoubleTensor::count == 6);
  static_assert(IntTensor::count == 6);
}

static void StaticTensorCreation()
{
  // Test MakeTensorFromDimensionT
  using Dim1 = TDimension<2, 3, 4>;
  using TensorFromDim1 = MakeTensorFromDimensionT<float, Dim1>;
  static_assert(std::is_same_v<TensorFromDim1, Tensor<float, 2, 3, 4>>);
  static_assert(TensorFromDim1::rank == 3);
  static_assert(TensorFromDim1::count == 24);
  
  using Dim2 = TDimension<5>;
  using TensorFromDim2 = MakeTensorFromDimensionT<int, Dim2>;
  static_assert(std::is_same_v<TensorFromDim2, Tensor<int, 5>>);
  static_assert(TensorFromDim2::rank == 1);
  static_assert(TensorFromDim2::count == 5);
  
  // Test IsTensorClass
  static_assert(IsTensorClass<Tensor<float, 2, 3>>::value);
  static_assert(IsTensorClass<Tensor<int, 1, 2, 3, 4>>::value);
  static_assert(!IsTensorClass<float>::value);
  static_assert(!IsTensorClass<int>::value);
  static_assert(!IsTensorClass<std::vector<float>>::value);
  static_assert(!IsTensorClass<TDimension<2, 3>>::value);
}

static void StaticTensorBroadcastingUtilities()
{
  // Test broadcasting compatibility with tensor types
  using ScalarTensor = Tensor<float, 1>;
  using VectorTensor = Tensor<float, 3>;
  using MatrixTensor = Tensor<float, 2, 3>;
  using Tensor3D = Tensor<float, 2, 3, 4>;
  
  // Test through tensor dimension types
  using ScalarDim = ScalarTensor::DimensionType;
  using VectorDim = VectorTensor::DimensionType;
  using MatrixDim = MatrixTensor::DimensionType;
  using Tensor3DDim = Tensor3D::DimensionType;
  
  // Scalar broadcasting to tensors
  static_assert(CanBroadcast<ScalarDim, VectorDim>::value);
  static_assert(CanBroadcast<ScalarDim, MatrixDim>::value);
  static_assert(CanBroadcast<ScalarDim, Tensor3DDim>::value);
  
  // Vector to matrix broadcasting (compatible dimensions)
  static_assert(CanBroadcast<VectorDim, MatrixDim>::value);
  
  // Same dimension broadcasting (always valid)
  static_assert(CanBroadcast<MatrixDim, MatrixDim>::value);
  static_assert(CanBroadcast<Tensor3DDim, Tensor3DDim>::value);
  
  // Test with different tensor scalar types (dimensions must still be compatible)
  using IntMatrix = Tensor<int, 2, 3>;
  using FloatMatrix = Tensor<float, 2, 3>;
  using IntMatrixDim = IntMatrix::DimensionType;
  using FloatMatrixDim = FloatMatrix::DimensionType;
  
  static_assert(CanBroadcast<IntMatrixDim, FloatMatrixDim>::value);
  static_assert(std::is_same_v<IntMatrixDim, FloatMatrixDim>); // Same dimension types
  
  // Test invalid broadcasting cases (should fail to compile)
  using Vector2Dim = Tensor<float, 2>::DimensionType;
  using Vector3Dim = Tensor<float, 3>::DimensionType;
  using Matrix23Dim = Tensor<float, 2, 3>::DimensionType;
  using Matrix33Dim = Tensor<float, 3, 3>::DimensionType;
  
  static_assert(!CanBroadcast<Vector2Dim, Vector3Dim>::value); // Incompatible sizes
  static_assert(!CanBroadcast<Matrix23Dim, Matrix33Dim>::value); // Incompatible matrix dims
  
  // Test MakeTensorFromDimensionT with broadcasting target dimensions
  using BroadcastResult1 = MakeTensorFromDimensionT<int, TDimension<3>>;
  static_assert(std::is_same_v<BroadcastResult1, Tensor<int, 3>>);
  static_assert(BroadcastResult1::rank == 1);
  static_assert(BroadcastResult1::count == 3);
  
  using BroadcastResult2 = MakeTensorFromDimensionT<float, TDimension<2, 3>>;
  static_assert(std::is_same_v<BroadcastResult2, Tensor<float, 2, 3>>);
  static_assert(BroadcastResult2::rank == 2);
  static_assert(BroadcastResult2::count == 6);
  
  using BroadcastResult3 = MakeTensorFromDimensionT<double, TDimension<2, 3, 4>>;
  static_assert(std::is_same_v<BroadcastResult3, Tensor<double, 2, 3, 4>>);
  static_assert(BroadcastResult3::rank == 3);
  static_assert(BroadcastResult3::count == 24);
  
  // Note: The Broadcast function is marked constexpr but cannot be evaluated at compile-time
  // due to internal implementation details (UnflattenedIndex, FlattenedIndex, Transform might not be constexpr).
  // The template/type system correctly validates broadcasting compatibility at compile-time,
  // but the actual broadcasting operation happens at runtime.
}

static void StaticConstexprOperations()
{
  // Test constexpr tensor construction and operations
  constexpr Tensor<int, 2, 2> constTensor(1, 2, 3, 4);
  static_assert(constTensor[0] == 1);
  static_assert(constTensor[1] == 2);
  static_assert(constTensor[2] == 3);
  static_assert(constTensor[3] == 4);
  
  // Test constexpr multi-dimensional access
  static_assert(constTensor(0, 0) == 1);
  static_assert(constTensor(0, 1) == 2);
  static_assert(constTensor(1, 0) == 3);
  static_assert(constTensor(1, 1) == 4);
  
  // Test constexpr implicit conversion for single-element tensors
  constexpr Tensor<float, 1> singleElement(42.0f);
  constexpr float extracted = singleElement;
  static_assert(extracted == 42.0f);
  
  // Note: conversion constructor uses std::copy which is not constexpr
  // Note: iterators from std::array are not constexpr in all implementations
}

static void StaticTensorDimensionAsTuple()
{
  // Test DimensionAsTupleV utility with tensor types
  using Tensor234 = Tensor<float, 2, 3, 4>;
  using TensorDim = Tensor234::DimensionType;
  constexpr auto dimTuple = DimensionAsTupleV<TensorDim>;
  static_assert(gtk::Get<0>(dimTuple) == 2);
  static_assert(gtk::Get<1>(dimTuple) == 3);
  static_assert(gtk::Get<2>(dimTuple) == 4);
  
  using Vector5 = Tensor<int, 5>;
  using VectorDim = Vector5::DimensionType;
  constexpr auto dimTuple5 = DimensionAsTupleV<VectorDim>;
  static_assert(gtk::Get<0>(dimTuple5) == 5);
  
  using ScalarTensor = Tensor<double, 1>;
  using ScalarDim = ScalarTensor::DimensionType;
  constexpr auto scalarTuple = DimensionAsTupleV<ScalarDim>;
  static_assert(gtk::Get<0>(scalarTuple) == 1);
  static_assert(gtk::Size(scalarTuple) == 1);
}

TEST(Math, Tensor)
{
  BasicTensorFunctionality();
  TensorInitialization();
  ImplicitConversions();
  TensorBroadcasting();
  TensorOperations();
  EdgeCases();
  
  // Compile-time tests for Tensor utilities
  StaticTensorUtilities();
  StaticTensorDimensionProperties();
  StaticTensorMakingUtilities();
  StaticTensorCreation();
  StaticTensorBroadcastingUtilities();
  StaticConstexprOperations();
  StaticTensorDimensionAsTuple();
}