#include <gtest/gtest.h>
#include <cmath>

#include "Vector.h"
#include "TensorOperations.h"


// Helper function for floating-point comparison of tensors
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

// Helper function for scalar comparison
template<typename T>
static bool AreScalarsNearlyEqual(T a, T b, T epsilon = T(1e-5))
{
  return std::abs(a - b) <= epsilon;
}

// Helper function to test vector construction and basic access
static void VectorConstruction()
{
  // Test 2D vector construction
  {
    Vector<float, 2> v2{1.0f, 2.0f};
    EXPECT_EQ(v2[0], 1.0f);
    EXPECT_EQ(v2[1], 2.0f);
  }

  // Test 3D vector construction
  {
    Vector<double, 3> v3{1.0, 2.0, 3.0};
    EXPECT_EQ(v3[0], 1.0);
    EXPECT_EQ(v3[1], 2.0);
    EXPECT_EQ(v3[2], 3.0);
  }

  // Test 4D vector construction
  {
    Vector<int, 4> v4{1, 2, 3, 4};
    EXPECT_EQ(v4[0], 1);
    EXPECT_EQ(v4[1], 2);
    EXPECT_EQ(v4[2], 3);
    EXPECT_EQ(v4[3], 4);
  }

  // Test default construction (zero vector)
  {
    Vector<float, 3> v;
    (void)v;  // Mark as intentionally unused
    // Note: Default construction might not initialize to zero, depending on implementation
    // This test verifies we can create a vector without arguments
  }
}

// Helper function to test vector arithmetic operations
static void ArithmeticOperations()
{
  Vector<float, 3> v1{1.0f, 2.0f, 3.0f};
  Vector<float, 3> v2{4.0f, 5.0f, 6.0f};

  // Test addition
  {
    auto result = v1 + v2;
    Vector<float, 3> expected{5.0f, 7.0f, 9.0f};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }

  // Test subtraction
  {
    auto result = v2 - v1;
    Vector<float, 3> expected{3.0f, 3.0f, 3.0f};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }

  // Test component-wise multiplication
  {
    auto result = v1 * v2;
    Vector<float, 3> expected{4.0f, 10.0f, 18.0f};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }

  // Test component-wise division
  {
    auto result = v2 / v1;
    Vector<float, 3> expected{4.0f, 2.5f, 2.0f};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }

  // Test scalar multiplication
  {
    auto result = v1 * 2.0f;
    Vector<float, 3> expected{2.0f, 4.0f, 6.0f};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }

  // Test scalar division
  {
    auto result = v2 / 2.0f;
    Vector<float, 3> expected{2.0f, 2.5f, 3.0f};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }
}

// Helper function to test dot product functionality
static void DotProduct()
{
  // Test 2D dot product
  {
    Vector<float, 2> v1{1.0f, 2.0f};
    Vector<float, 2> v2{3.0f, 4.0f};
    auto result = Dot(v1, v2);
    float expected = 1.0f * 3.0f + 2.0f * 4.0f;  // 11.0f
    EXPECT_TRUE(AreScalarsNearlyEqual(result, expected));
  }

  // Test 3D dot product
  {
    Vector<double, 3> v1{1.0, 2.0, 3.0};
    Vector<double, 3> v2{4.0, 5.0, 6.0};
    auto result = Dot(v1, v2);
    double expected = 1.0 * 4.0 + 2.0 * 5.0 + 3.0 * 6.0;  // 32.0
    EXPECT_TRUE(AreScalarsNearlyEqual(result, expected));
  }

  // Test orthogonal vectors (dot product should be zero)
  {
    Vector<float, 3> v1{1.0f, 0.0f, 0.0f};
    Vector<float, 3> v2{0.0f, 1.0f, 0.0f};
    auto result = Dot(v1, v2);
    EXPECT_TRUE(AreScalarsNearlyEqual(result, 0.0f));
  }

  // Test parallel vectors
  {
    Vector<float, 3> v1{2.0f, 3.0f, 4.0f};
    Vector<float, 3> v2{4.0f, 6.0f, 8.0f};  // v2 = 2 * v1
    auto v1_dot_v1 = Dot(v1, v1);
    auto result = Dot(v1, v2);
    auto expected = 2.0f * v1_dot_v1;
    EXPECT_TRUE(AreScalarsNearlyEqual(result, expected));
  }
}

// Helper function to test cross product functionality (3D only)
static void CrossProduct()
{
  // Test standard unit vectors
  {
    Vector<float, 3> i{1.0f, 0.0f, 0.0f};
    Vector<float, 3> j{0.0f, 1.0f, 0.0f};
    Vector<float, 3> k{0.0f, 0.0f, 1.0f};

    // i × j = k
    auto result1 = Cross(i, j);
    EXPECT_TRUE(AreNearlyEqual(result1, k));

    // j × k = i
    auto result2 = Cross(j, k);
    EXPECT_TRUE(AreNearlyEqual(result2, i));

    // k × i = j
    auto result3 = Cross(k, i);
    EXPECT_TRUE(AreNearlyEqual(result3, j));
  }

  // Test anti-commutativity: a × b = -(b × a)
  {
    Vector<double, 3> v1{1.0, 2.0, 3.0};
    Vector<double, 3> v2{4.0, 5.0, 6.0};

    auto cross1 = Cross(v1, v2);
    auto cross2 = Cross(v2, v1);
    auto negated_cross2 = Vector<double, 3>{-cross2[0], -cross2[1], -cross2[2]};

    EXPECT_TRUE(AreNearlyEqual(cross1, negated_cross2));
  }

  // Test cross product of parallel vectors (should be zero)
  {
    Vector<float, 3> v1{2.0f, 4.0f, 6.0f};
    Vector<float, 3> v2{1.0f, 2.0f, 3.0f};  // parallel to v1

    auto result = Cross(v1, v2);
    Vector<float, 3> zero{0.0f, 0.0f, 0.0f};
    EXPECT_TRUE(AreNearlyEqual(result, zero));
  }

  // Test cross product orthogonality
  {
    Vector<float, 3> v1{1.0f, 2.0f, 3.0f};
    Vector<float, 3> v2{4.0f, 5.0f, 6.0f};

    auto cross_result = Cross(v1, v2);

    // Cross product should be orthogonal to both input vectors
    auto dot1 = Dot(cross_result, v1);
    auto dot2 = Dot(cross_result, v2);

    EXPECT_TRUE(AreScalarsNearlyEqual(dot1, 0.0f));
    EXPECT_TRUE(AreScalarsNearlyEqual(dot2, 0.0f));
  }

  // Test specific cross product calculation
  {
    Vector<int, 3> v1{1, 2, 3};
    Vector<int, 3> v2{4, 5, 6};

    auto result = Cross(v1, v2);
    // Expected: [2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4] = [-3, 6, -3]
    Vector<int, 3> expected{-3, 6, -3};
    EXPECT_TRUE(AreNearlyEqual(result, expected));
  }
}

// Helper function to test vector utility functions and properties
static void VectorProperties()
{
  // Test vector magnitude/length via dot product
  {
    Vector<float, 3> v{3.0f, 4.0f, 0.0f};
    auto dot_result = Dot(v, v);
    auto length_squared = 3.0f * 3.0f + 4.0f * 4.0f;  // 25.0f
    EXPECT_TRUE(AreScalarsNearlyEqual(dot_result, length_squared));
  }

  // Test unit vector properties
  {
    Vector<double, 3> unit_x{1.0, 0.0, 0.0};
    auto magnitude_squared = Dot(unit_x, unit_x);
    EXPECT_TRUE(AreScalarsNearlyEqual(magnitude_squared, 1.0));
  }
}

// Helper function to test edge cases and special vectors
static void EdgeCases()
{
  // Test zero vector
  {
    Vector<float, 3> zero{0.0f, 0.0f, 0.0f};
    Vector<float, 3> other{1.0f, 2.0f, 3.0f};

    // Zero vector dot product
    auto dot_result = Dot(zero, other);
    EXPECT_TRUE(AreScalarsNearlyEqual(dot_result, 0.0f));

    // Zero vector cross product
    auto cross_result = Cross(zero, other);
    EXPECT_TRUE(AreNearlyEqual(cross_result, zero));
  }

  // Test single element vector
  {
    Vector<double, 1> v1{5.0};
    Vector<double, 1> v2{3.0};

    auto dot_result = Dot(v1, v2);
    EXPECT_TRUE(AreScalarsNearlyEqual(dot_result, 15.0));
  }

  // Test large dimension vector
  {
    Vector<float, 5> v1{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    Vector<float, 5> v2{5.0f, 4.0f, 3.0f, 2.0f, 1.0f};

    auto result = Dot(v1, v2);
    float expected = 1 * 5 + 2 * 4 + 3 * 3 + 4 * 2 + 5 * 1;  // 35.0f
    EXPECT_TRUE(AreScalarsNearlyEqual(result, expected));
  }
}

// Helper function to test type conversions and mixed operations
static void TypeConversions()
{
  // Test different scalar types
  {
    Vector<float, 3> v_float{1.0f, 2.0f, 3.0f};
    Vector<double, 3> v_double{1.0, 2.0, 3.0};

    // These should be nearly equal despite different types
    EXPECT_TRUE(AreNearlyEqual(v_float, v_double));
  }

  // Test integer vectors
  {
    Vector<int, 2> v1{3, 4};
    Vector<int, 2> v2{1, 2};

    auto dot_result = Dot(v1, v2);
    int expected = 3 * 1 + 4 * 2;  // 11
    EXPECT_EQ(dot_result, expected);
  }
}

// Helper function to test comprehensive operations combining multiple vector operations
static void ComprehensiveOperations()
{
  // Test a realistic 3D graphics scenario
  {
    // Define some 3D vectors representing points/directions in space
    Vector<float, 3> position{1.0f, 2.0f, 3.0f};
    Vector<float, 3> velocity{0.5f, -1.0f, 0.2f};
    Vector<float, 3> force{0.1f, 0.0f, -0.3f};
    
    // Simulate basic physics: new_position = position + velocity * time
    float time = 2.0f;
    auto new_position = position + velocity * time;
    Vector<float, 3> expected_position{2.0f, 0.0f, 3.4f};
    EXPECT_TRUE(AreNearlyEqual(new_position, expected_position));
    
    // Calculate acceleration from force (assuming unit mass)
    auto acceleration = force;
    
    // Update velocity: new_velocity = velocity + acceleration * time
    auto new_velocity = velocity + acceleration * time;
    Vector<float, 3> expected_velocity{0.7f, -1.0f, -0.4f};
    EXPECT_TRUE(AreNearlyEqual(new_velocity, expected_velocity));
    
    // Calculate kinetic energy: KE = 0.5 * mass * |velocity|^2
    auto velocity_squared = Dot(velocity, velocity);
    float mass = 2.0f;
    auto kinetic_energy = 0.5f * mass * velocity_squared;
    float expected_ke = 0.5f * 2.0f * (0.5f*0.5f + (-1.0f)*(-1.0f) + 0.2f*0.2f);
    EXPECT_TRUE(AreScalarsNearlyEqual(kinetic_energy, expected_ke));
  }
  
  // Test vector geometry operations
  {
    Vector<double, 3> a{1.0, 0.0, 0.0};
    Vector<double, 3> b{0.0, 1.0, 0.0};
    Vector<double, 3> c{0.0, 0.0, 1.0};
    
    // Test that cross products form an orthonormal basis
    auto cross_ab = Cross(a, b);
    auto cross_bc = Cross(b, c);
    auto cross_ca = Cross(c, a);
    
    EXPECT_TRUE(AreNearlyEqual(cross_ab, c));
    EXPECT_TRUE(AreNearlyEqual(cross_bc, a));
    EXPECT_TRUE(AreNearlyEqual(cross_ca, b));
    
    // Test scalar triple product: a · (b × c) = 1 (volume of unit cube)
    auto scalar_triple = Dot(a, Cross(b, c));
    EXPECT_TRUE(AreScalarsNearlyEqual(scalar_triple, 1.0));
    
    // Test vector triple product identity: a × (b × c) = b(a·c) - c(a·b)
    Vector<double, 3> u{2.0, 1.0, 3.0};
    Vector<double, 3> v{1.0, 2.0, 1.0};
    Vector<double, 3> w{3.0, 1.0, 2.0};
    
    auto cross_vw = Cross(v, w);
    auto triple_product = Cross(u, cross_vw);
    
    auto dot_uw = Dot(u, w);
    auto dot_uv = Dot(u, v);
    auto identity_result = v * dot_uw - w * dot_uv;
    
    EXPECT_TRUE(AreNearlyEqual(triple_product, identity_result));
  }
  
  // Test mixed operations with different vector dimensions
  {
    Vector<float, 2> v2d{3.0f, 4.0f};
    Vector<float, 4> v4d{1.0f, 2.0f, 3.0f, 4.0f};
    
    // Test 2D vector magnitude calculation
    auto magnitude_2d_squared = Dot(v2d, v2d);
    float expected_mag_sq = 3.0f*3.0f + 4.0f*4.0f; // 25.0f
    EXPECT_TRUE(AreScalarsNearlyEqual(magnitude_2d_squared, expected_mag_sq));
    
    // Test 4D vector operations
    Vector<float, 4> v4d_other{4.0f, 3.0f, 2.0f, 1.0f};
    auto dot_4d = Dot(v4d, v4d_other);
    float expected_dot_4d = 1*4 + 2*3 + 3*2 + 4*1; // 20.0f
    EXPECT_TRUE(AreScalarsNearlyEqual(dot_4d, expected_dot_4d));
    
    // Test componentwise operations in 4D
    auto sum_4d = v4d + v4d_other;
    Vector<float, 4> expected_sum{5.0f, 5.0f, 5.0f, 5.0f};
    EXPECT_TRUE(AreNearlyEqual(sum_4d, expected_sum));
  }
}

// Single comprehensive test that calls all individual test functions
TEST(Math, Vector)
{
  VectorConstruction();
  ArithmeticOperations();
  DotProduct();
  CrossProduct();
  VectorProperties();
  EdgeCases();
  TypeConversions();
  ComprehensiveOperations();
}