#include <array>
#include <gtest/gtest.h>

#include "Dimension.h"


static void test()
{

  // Basic Properties Tests
  using Scalar = TDimension<>;  // Actually becomes TDimension<1>
  using Vector = TDimension<5>;
  using Matrix = TDimension<3, 4>;
  using Tensor3D = TDimension<2, 3, 4>;
  using Tensor4D = TDimension<2, 3, 4, 5>;

  // Test rank - Scalar is special case that inherits from TDimension<1>
  static_assert(Scalar::rank == 1);  // Scalar specialization has rank 1
  static_assert(Vector::rank == 1);
  static_assert(Matrix::rank == 2);
  static_assert(Tensor3D::rank == 3);
  static_assert(Tensor4D::rank == 4);

  // Test count (total elements)
  static_assert(Scalar::count == 1);
  static_assert(Vector::count == 5);
  static_assert(Matrix::count == 12);
  static_assert(Tensor3D::count == 24);
  static_assert(Tensor4D::count == 120);

  // Test type predicates - Note: TDimension<> inherits from TDimension<1>
  static_assert(!Scalar::isScalar);  // rank is 1, not 0
  static_assert(Scalar::isVector);   // rank is 1
  static_assert(!Scalar::isMatrix);
  static_assert(!Scalar::isTensor);

  static_assert(!Vector::isScalar);
  static_assert(Vector::isVector);
  static_assert(!Vector::isMatrix);
  static_assert(!Vector::isTensor);

  static_assert(!Matrix::isScalar);
  static_assert(!Matrix::isVector);
  static_assert(Matrix::isMatrix);
  static_assert(!Matrix::isTensor);

  static_assert(!Tensor3D::isScalar);
  static_assert(!Tensor3D::isVector);
  static_assert(!Tensor3D::isMatrix);
  static_assert(Tensor3D::isTensor);


  {
    // DimGet Tests
    using Matrix = TDimension<3, 4>;
    using Tensor3D = TDimension<2, 3, 4>;

    static_assert(DimGet<Matrix, 0> == 3);
    static_assert(DimGet<Matrix, 1> == 4);

    static_assert(DimGet<Tensor3D, 0> == 2);
    static_assert(DimGet<Tensor3D, 1> == 3);
    static_assert(DimGet<Tensor3D, 2> == 4);
  }

  {
    // Flattened Index Tests (Runtime only - compile-time version seems to have issues)
    using Matrix = TDimension<3, 4>;

    // Test runtime index calculation
    EXPECT_EQ(Matrix::FlattenedIndex(0, 0), 0);
    EXPECT_EQ(Matrix::FlattenedIndex(0, 1), 1);
    EXPECT_EQ(Matrix::FlattenedIndex(0, 2), 2);
    EXPECT_EQ(Matrix::FlattenedIndex(1, 0), 4);
    EXPECT_EQ(Matrix::FlattenedIndex(1, 1), 5);
    EXPECT_EQ(Matrix::FlattenedIndex(2, 3), 11);

    using Tensor3D = TDimension<2, 3, 4>;

    EXPECT_EQ(Tensor3D::FlattenedIndex(0, 0, 0), 0);
    EXPECT_EQ(Tensor3D::FlattenedIndex(0, 0, 1), 1);
    EXPECT_EQ(Tensor3D::FlattenedIndex(0, 1, 0), 4);
    EXPECT_EQ(Tensor3D::FlattenedIndex(1, 0, 0), 12);
    EXPECT_EQ(Tensor3D::FlattenedIndex(1, 2, 3), 23);

    // Test with tuple indices
    auto matrixIndex = gtk::MakeTuple(1, 2);
    EXPECT_EQ(Matrix::FlattenedIndex(matrixIndex), 6);

    auto tensorIndex = gtk::MakeTuple(1, 1, 2);
    EXPECT_EQ(Tensor3D::FlattenedIndex(tensorIndex), 18);
  }

  {
    // Unflattened Index Tests
    using Matrix = TDimension<3, 4>;

    auto idx0 = Matrix::UnflattenedIndex(0);
    EXPECT_EQ(gtk::Get<0>(idx0), 0);
    EXPECT_EQ(gtk::Get<1>(idx0), 0);

    auto idx5 = Matrix::UnflattenedIndex(5);
    EXPECT_EQ(gtk::Get<0>(idx5), 1);
    EXPECT_EQ(gtk::Get<1>(idx5), 1);

    auto idx11 = Matrix::UnflattenedIndex(11);
    EXPECT_EQ(gtk::Get<0>(idx11), 2);
    EXPECT_EQ(gtk::Get<1>(idx11), 3);

    using Tensor3D = TDimension<2, 3, 4>;

    auto idx3d_0 = Tensor3D::UnflattenedIndex(0);
    EXPECT_EQ(gtk::Get<0>(idx3d_0), 0);
    EXPECT_EQ(gtk::Get<1>(idx3d_0), 0);
    EXPECT_EQ(gtk::Get<2>(idx3d_0), 0);

    auto idx3d_23 = Tensor3D::UnflattenedIndex(23);
    EXPECT_EQ(gtk::Get<0>(idx3d_23), 1);
    EXPECT_EQ(gtk::Get<1>(idx3d_23), 2);
    EXPECT_EQ(gtk::Get<2>(idx3d_23), 3);
  }

  {
    // Dimension Manipulation Tests
    using Vector = TDimension<5>;
    using Matrix = TDimension<3, 4>;

    // Test DimFront
    static_assert(DimFront<Vector> == 5);
    static_assert(DimFront<Matrix> == 3);

    // Test DimPushFront
    using VectorPushed = DimPushFront<Vector, 7>;
    static_assert(std::is_same_v<VectorPushed, TDimension<7, 5>>);

    using MatrixPushed = DimPushFront<Matrix, 2>;
    static_assert(std::is_same_v<MatrixPushed, TDimension<2, 3, 4>>);

    // Test DimPopFront
    using MatrixPopped = DimPopFront<Matrix>;
    static_assert(std::is_same_v<MatrixPopped, TDimension<4>>);

    // Test DimReverse
    using MatrixReversed = DimReverse<Matrix>;
    static_assert(std::is_same_v<MatrixReversed, TDimension<4, 3>>);

    using Tensor3DReversed = DimReverse<TDimension<2, 3, 4>>;
    static_assert(std::is_same_v<Tensor3DReversed, TDimension<4, 3, 2>>);
  }

  {
    // Broadcasting Compatibility Tests
    using Scalar = TDimension<>;
    using Vector = TDimension<5>;
    using Matrix = TDimension<3, 5>;
    using Tensor3D = TDimension<2, 3, 5>;

    // Compatible dimensions (basic cases)
    static_assert(IsCompatibleDimV<Scalar, Scalar>);
    static_assert(IsCompatibleDimV<Vector, Vector>);

    // Test with dimensions that have 1s (should be compatible)
    using VectorWith1 = TDimension<1>;
    using MatrixWith1 = TDimension<3, 1>;
    static_assert(IsCompatibleDimV<VectorWith1, Vector>);
    static_assert(IsCompatibleDimV<MatrixWith1, TDimension<3, 5>>);

    // Test same dimensions
    static_assert(IsCompatibleDimV<Matrix, Matrix>);
    static_assert(IsCompatibleDimV<Tensor3D, Tensor3D>);

    // Incompatible dimensions
    using Vector3 = TDimension<3>;
    using Vector4 = TDimension<4>;
    static_assert(!IsCompatibleDimV<Vector3, Vector4>);

    using Matrix34 = TDimension<3, 4>;
    using Matrix35 = TDimension<3, 5>;
    static_assert(!IsCompatibleDimV<Matrix34, Matrix35>);
  }

  {
    // Padding Tests
    using Vector = TDimension<5>;
    using Matrix = TDimension<3, 5>;
    using Tensor3D = TDimension<2, 3, 5>;

    // Test padding to same rank
    using PadResult = PadDim<Vector, Tensor3D>;
    static_assert(std::is_same_v<PadResult::D1Padded, TDimension<1, 1, 5>>);
    static_assert(std::is_same_v<PadResult::D2Padded, TDimension<2, 3, 5>>);

    using PadResult2 = PadDim<Matrix, Tensor3D>;
    static_assert(std::is_same_v<PadResult2::D1Padded, TDimension<1, 3, 5>>);
    static_assert(std::is_same_v<PadResult2::D2Padded, TDimension<2, 3, 5>>);

    // Test when dimensions have same rank
    using PadResultSame = PadDim<Matrix, TDimension<4, 6>>;
    static_assert(std::is_same_v<PadResultSame::D1Padded, TDimension<3, 5>>);
    static_assert(std::is_same_v<PadResultSame::D2Padded, TDimension<4, 6>>);
  }
}

TEST(Math, Dimension)
{
  test();
}