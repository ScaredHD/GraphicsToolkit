#include <array>
#include <gtest/gtest.h>

#include "Dimension.h"

static void test()
{
  // Common type aliases used throughout tests
  using Scalar = TDimension<>;  // True scalar with rank 0
  using Vector = TDimension<5>;
  using Matrix = TDimension<3, 4>;
  using Tensor3D = TDimension<2, 3, 4>;
  using Tensor4D = TDimension<2, 3, 4, 5>;

  // Additional corner case types
  using SingleElementVector = TDimension<1>;
  using LargeVector = TDimension<1000>;
  using SingleRowMatrix = TDimension<1, 10>;
  using SingleColMatrix = TDimension<10, 1>;
  using SquareMatrix = TDimension<5, 5>;
  using LargeTensor = TDimension<10, 20, 30, 40>;
  using MixedTensor = TDimension<1, 5, 1, 3>;
  using LargeRankTensor = TDimension<2, 2, 2, 2, 2, 2>;  // 6D tensor

  {
    // Basic Properties Tests (Enhanced with corner cases)
    // Test rank - including edge cases
    static_assert(Scalar::rank == 0);  // Scalar specialization has rank 0
    static_assert(Vector::rank == 1);
    static_assert(Matrix::rank == 2);
    static_assert(Tensor3D::rank == 3);
    static_assert(Tensor4D::rank == 4);
    static_assert(LargeRankTensor::rank == 6);

    // Test count (total elements) - including large numbers
    static_assert(Scalar::count == 1);
    static_assert(Vector::count == 5);
    static_assert(SingleElementVector::count == 1);
    static_assert(LargeVector::count == 1000);
    static_assert(Matrix::count == 12);
    static_assert(SingleRowMatrix::count == 10);
    static_assert(SingleColMatrix::count == 10);
    static_assert(SquareMatrix::count == 25);
    static_assert(Tensor3D::count == 24);
    static_assert(Tensor4D::count == 120);
    static_assert(LargeTensor::count == 240000);  // 10*20*30*40
    static_assert(MixedTensor::count == 15);      // 1*5*1*3
    static_assert(LargeRankTensor::count == 64);  // 2^6

    // Test type predicates - enhanced with corner cases
    static_assert(Scalar::isScalar);   // rank is 0
    static_assert(!Scalar::isVector);  // rank is not 1
    static_assert(!Scalar::isMatrix);
    static_assert(!Scalar::isTensor);

    static_assert(!Vector::isScalar);
    static_assert(Vector::isVector);
    static_assert(!Vector::isMatrix);
    static_assert(!Vector::isTensor);

    // Corner case: single element vector is still a vector
    static_assert(!SingleElementVector::isScalar);
    static_assert(SingleElementVector::isVector);
    static_assert(!SingleElementVector::isMatrix);
    static_assert(!SingleElementVector::isTensor);

    static_assert(!Matrix::isScalar);
    static_assert(!Matrix::isVector);
    static_assert(Matrix::isMatrix);
    static_assert(!Matrix::isTensor);

    // Corner cases: degenerate matrices are still matrices
    static_assert(!SingleRowMatrix::isScalar);
    static_assert(!SingleRowMatrix::isVector);
    static_assert(SingleRowMatrix::isMatrix);
    static_assert(!SingleRowMatrix::isTensor);

    static_assert(!SingleColMatrix::isScalar);
    static_assert(!SingleColMatrix::isVector);
    static_assert(SingleColMatrix::isMatrix);
    static_assert(!SingleColMatrix::isTensor);

    static_assert(!Tensor3D::isScalar);
    static_assert(!Tensor3D::isVector);
    static_assert(!Tensor3D::isMatrix);
    static_assert(Tensor3D::isTensor);

    // Higher-rank tensors
    static_assert(!LargeRankTensor::isScalar);
    static_assert(!LargeRankTensor::isVector);
    static_assert(!LargeRankTensor::isMatrix);
    static_assert(LargeRankTensor::isTensor);
  }

  {
    // DimGet Tests (Enhanced with corner cases)
    static_assert(DimGet<Matrix, 0> == 3);
    static_assert(DimGet<Matrix, 1> == 4);
    static_assert(DimGet<Tensor3D, 0> == 2);
    static_assert(DimGet<Tensor3D, 1> == 3);
    static_assert(DimGet<Tensor3D, 2> == 4);

    // Corner cases
    static_assert(DimGet<SingleElementVector, 0> == 1);
    static_assert(DimGet<LargeVector, 0> == 1000);
    static_assert(DimGet<SingleRowMatrix, 0> == 1);
    static_assert(DimGet<SingleRowMatrix, 1> == 10);
    static_assert(DimGet<SingleColMatrix, 0> == 10);
    static_assert(DimGet<SingleColMatrix, 1> == 1);
    static_assert(DimGet<MixedTensor, 0> == 1);
    static_assert(DimGet<MixedTensor, 1> == 5);
    static_assert(DimGet<MixedTensor, 2> == 1);
    static_assert(DimGet<MixedTensor, 3> == 3);
    static_assert(DimGet<LargeRankTensor, 0> == 2);
    static_assert(DimGet<LargeRankTensor, 5> == 2);  // Last dimension
  }

  {
    // Compile-time Flattened Index Tests (Enhanced with corner cases)
    // Test scalar flattened index - should always be 0
    static_assert(Scalar::FlattenedIndex() == 0);

    // Test corner case: single element vector
    static_assert(SingleElementVector::FlattenedIndex<0>() == 0);

    // Test compile-time matrix indexing
    static_assert(Matrix::FlattenedIndex<0, 0>() == 0);
    static_assert(Matrix::FlattenedIndex<0, 1>() == 1);
    static_assert(Matrix::FlattenedIndex<0, 2>() == 2);
    static_assert(Matrix::FlattenedIndex<0, 3>() == 3);
    static_assert(Matrix::FlattenedIndex<1, 0>() == 4);
    static_assert(Matrix::FlattenedIndex<1, 1>() == 5);
    static_assert(Matrix::FlattenedIndex<1, 2>() == 6);
    static_assert(Matrix::FlattenedIndex<2, 0>() == 8);
    static_assert(Matrix::FlattenedIndex<2, 3>() == 11);

    // Test corner cases: degenerate matrices
    static_assert(SingleRowMatrix::FlattenedIndex<0, 0>() == 0);
    static_assert(SingleRowMatrix::FlattenedIndex<0, 9>() == 9);  // Last element
    static_assert(SingleColMatrix::FlattenedIndex<0, 0>() == 0);
    static_assert(SingleColMatrix::FlattenedIndex<9, 0>() == 9);  // Last element

    // Test square matrix boundary cases
    static_assert(SquareMatrix::FlattenedIndex<0, 0>() == 0);   // First element
    static_assert(SquareMatrix::FlattenedIndex<4, 4>() == 24);  // Last element
    static_assert(SquareMatrix::FlattenedIndex<2, 2>() == 12);  // Middle element

    // Test compile-time 3D tensor indexing
    static_assert(Tensor3D::FlattenedIndex<0, 0, 0>() == 0);
    static_assert(Tensor3D::FlattenedIndex<0, 0, 1>() == 1);
    static_assert(Tensor3D::FlattenedIndex<0, 0, 2>() == 2);
    static_assert(Tensor3D::FlattenedIndex<0, 1, 0>() == 4);
    static_assert(Tensor3D::FlattenedIndex<0, 2, 0>() == 8);
    static_assert(Tensor3D::FlattenedIndex<1, 0, 0>() == 12);
    static_assert(Tensor3D::FlattenedIndex<1, 1, 1>() == 17);
    static_assert(Tensor3D::FlattenedIndex<1, 2, 3>() == 23);

    // Test vector indexing including boundary cases
    static_assert(Vector::FlattenedIndex<0>() == 0);
    static_assert(Vector::FlattenedIndex<1>() == 1);
    static_assert(Vector::FlattenedIndex<4>() == 4);

    // Test 4D tensor indexing including boundary cases
    static_assert(Tensor4D::FlattenedIndex<0, 0, 0, 0>() == 0);
    static_assert(Tensor4D::FlattenedIndex<0, 0, 0, 1>() == 1);
    static_assert(Tensor4D::FlattenedIndex<0, 0, 1, 0>() == 5);
    static_assert(Tensor4D::FlattenedIndex<0, 1, 0, 0>() == 20);
    static_assert(Tensor4D::FlattenedIndex<1, 0, 0, 0>() == 60);
    static_assert(Tensor4D::FlattenedIndex<1, 2, 3, 4>() == 119);  // Last element

    // Test mixed tensor with 1-dimensions
    static_assert(MixedTensor::FlattenedIndex<0, 0, 0, 0>() == 0);
    static_assert(MixedTensor::FlattenedIndex<0, 4, 0, 2>() == 14);  // Last element
    static_assert(MixedTensor::FlattenedIndex<0, 2, 0, 1>() == 7);   // Middle element

    // Test high-rank tensor boundary cases
    static_assert(LargeRankTensor::FlattenedIndex<0, 0, 0, 0, 0, 0>() == 0);   // First element
    static_assert(LargeRankTensor::FlattenedIndex<1, 1, 1, 1, 1, 1>() == 63);  // Last element
  }

  {
    // Runtime Flattened Index Tests (Enhanced with corner cases)
    // Test scalar runtime index calculation
    EXPECT_EQ(Scalar::FlattenedIndex(), 0);

    // Test empty tuple for scalar
    auto emptyTuple = gtk::MakeTuple();
    EXPECT_EQ(Scalar::FlattenedIndex(emptyTuple), 0);

    // Test corner case: single element vector
    EXPECT_EQ(SingleElementVector::FlattenedIndex(0), 0);
    auto singleTuple = gtk::MakeTuple(0);
    EXPECT_EQ(SingleElementVector::FlattenedIndex(singleTuple), 0);

    // Test runtime index calculation for matrices
    EXPECT_EQ(Matrix::FlattenedIndex(0, 0), 0);
    EXPECT_EQ(Matrix::FlattenedIndex(0, 1), 1);
    EXPECT_EQ(Matrix::FlattenedIndex(0, 2), 2);
    EXPECT_EQ(Matrix::FlattenedIndex(1, 0), 4);
    EXPECT_EQ(Matrix::FlattenedIndex(1, 1), 5);
    EXPECT_EQ(Matrix::FlattenedIndex(2, 3), 11);

    // Test corner cases: degenerate matrices
    EXPECT_EQ(SingleRowMatrix::FlattenedIndex(0, 0), 0);
    EXPECT_EQ(SingleRowMatrix::FlattenedIndex(0, 9), 9);  // Last element
    EXPECT_EQ(SingleColMatrix::FlattenedIndex(0, 0), 0);
    EXPECT_EQ(SingleColMatrix::FlattenedIndex(9, 0), 9);  // Last element

    // Test square matrix boundary cases
    EXPECT_EQ(SquareMatrix::FlattenedIndex(0, 0), 0);   // First element
    EXPECT_EQ(SquareMatrix::FlattenedIndex(4, 4), 24);  // Last element
    EXPECT_EQ(SquareMatrix::FlattenedIndex(2, 2), 12);  // Center element

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

    // Test corner case tuples
    auto singleRowIndex = gtk::MakeTuple(0, 5);
    EXPECT_EQ(SingleRowMatrix::FlattenedIndex(singleRowIndex), 5);

    auto singleColIndex = gtk::MakeTuple(5, 0);
    EXPECT_EQ(SingleColMatrix::FlattenedIndex(singleColIndex), 5);

    // Test mixed tensor with 1-dimensions
    auto mixedIndex = gtk::MakeTuple(0, 3, 0, 1);
    EXPECT_EQ(MixedTensor::FlattenedIndex(mixedIndex), 10);

    // Test high-rank tensor
    auto largeRankIndex = gtk::MakeTuple(1, 0, 1, 0, 1, 0);
    EXPECT_EQ(LargeRankTensor::FlattenedIndex(largeRankIndex), 42);  // Calculated: 32 + 8 + 2 = 42
  }

  {
    // Unflattened Index Tests (Enhanced with corner cases)
    // Test scalar unflattened index - should return empty tuple
    auto scalarIdx = Scalar::UnflattenedIndex(0);
    (void)scalarIdx;  // Suppress unused variable warning

    // Test scalar unflattened index with other values (should still be empty)
    auto scalarIdx2 = Scalar::UnflattenedIndex(42);
    (void)scalarIdx2;  // Suppress unused variable warning

    // Test corner case: single element vector
    auto singleVectorIdx = SingleElementVector::UnflattenedIndex(0);
    EXPECT_EQ(gtk::Get<0>(singleVectorIdx), 0);

    // Test matrix unflattened index
    auto idx0 = Matrix::UnflattenedIndex(0);
    EXPECT_EQ(gtk::Get<0>(idx0), 0);
    EXPECT_EQ(gtk::Get<1>(idx0), 0);

    auto idx5 = Matrix::UnflattenedIndex(5);
    EXPECT_EQ(gtk::Get<0>(idx5), 1);
    EXPECT_EQ(gtk::Get<1>(idx5), 1);

    auto idx11 = Matrix::UnflattenedIndex(11);
    EXPECT_EQ(gtk::Get<0>(idx11), 2);
    EXPECT_EQ(gtk::Get<1>(idx11), 3);

    // Test corner cases: degenerate matrices
    auto singleRowIdx5 = SingleRowMatrix::UnflattenedIndex(5);
    EXPECT_EQ(gtk::Get<0>(singleRowIdx5), 0);
    EXPECT_EQ(gtk::Get<1>(singleRowIdx5), 5);

    auto singleRowIdx9 = SingleRowMatrix::UnflattenedIndex(9);  // Last element
    EXPECT_EQ(gtk::Get<0>(singleRowIdx9), 0);
    EXPECT_EQ(gtk::Get<1>(singleRowIdx9), 9);

    auto singleColIdx5 = SingleColMatrix::UnflattenedIndex(5);
    EXPECT_EQ(gtk::Get<0>(singleColIdx5), 5);
    EXPECT_EQ(gtk::Get<1>(singleColIdx5), 0);

    auto singleColIdx9 = SingleColMatrix::UnflattenedIndex(9);  // Last element
    EXPECT_EQ(gtk::Get<0>(singleColIdx9), 9);
    EXPECT_EQ(gtk::Get<1>(singleColIdx9), 0);

    // Test square matrix boundary cases
    auto squareFirst = SquareMatrix::UnflattenedIndex(0);  // First element
    EXPECT_EQ(gtk::Get<0>(squareFirst), 0);
    EXPECT_EQ(gtk::Get<1>(squareFirst), 0);

    auto squareLast = SquareMatrix::UnflattenedIndex(24);  // Last element
    EXPECT_EQ(gtk::Get<0>(squareLast), 4);
    EXPECT_EQ(gtk::Get<1>(squareLast), 4);

    auto squareCenter = SquareMatrix::UnflattenedIndex(12);  // Center element
    EXPECT_EQ(gtk::Get<0>(squareCenter), 2);
    EXPECT_EQ(gtk::Get<1>(squareCenter), 2);

    auto idx3d_0 = Tensor3D::UnflattenedIndex(0);
    EXPECT_EQ(gtk::Get<0>(idx3d_0), 0);
    EXPECT_EQ(gtk::Get<1>(idx3d_0), 0);
    EXPECT_EQ(gtk::Get<2>(idx3d_0), 0);

    auto idx3d_23 = Tensor3D::UnflattenedIndex(23);
    EXPECT_EQ(gtk::Get<0>(idx3d_23), 1);
    EXPECT_EQ(gtk::Get<1>(idx3d_23), 2);
    EXPECT_EQ(gtk::Get<2>(idx3d_23), 3);

    // Test mixed tensor corner cases
    auto mixedFirst = MixedTensor::UnflattenedIndex(0);  // First element
    EXPECT_EQ(gtk::Get<0>(mixedFirst), 0);
    EXPECT_EQ(gtk::Get<1>(mixedFirst), 0);
    EXPECT_EQ(gtk::Get<2>(mixedFirst), 0);
    EXPECT_EQ(gtk::Get<3>(mixedFirst), 0);

    auto mixedLast = MixedTensor::UnflattenedIndex(14);  // Last element
    EXPECT_EQ(gtk::Get<0>(mixedLast), 0);
    EXPECT_EQ(gtk::Get<1>(mixedLast), 4);
    EXPECT_EQ(gtk::Get<2>(mixedLast), 0);
    EXPECT_EQ(gtk::Get<3>(mixedLast), 2);

    // Test high-rank tensor boundary cases
    auto largeRankFirst = LargeRankTensor::UnflattenedIndex(0);  // First element
    EXPECT_EQ(gtk::Get<0>(largeRankFirst), 0);
    EXPECT_EQ(gtk::Get<1>(largeRankFirst), 0);
    EXPECT_EQ(gtk::Get<2>(largeRankFirst), 0);
    EXPECT_EQ(gtk::Get<3>(largeRankFirst), 0);
    EXPECT_EQ(gtk::Get<4>(largeRankFirst), 0);
    EXPECT_EQ(gtk::Get<5>(largeRankFirst), 0);

    auto largeRankLast = LargeRankTensor::UnflattenedIndex(63);  // Last element
    EXPECT_EQ(gtk::Get<0>(largeRankLast), 1);
    EXPECT_EQ(gtk::Get<1>(largeRankLast), 1);
    EXPECT_EQ(gtk::Get<2>(largeRankLast), 1);
    EXPECT_EQ(gtk::Get<3>(largeRankLast), 1);
    EXPECT_EQ(gtk::Get<4>(largeRankLast), 1);
    EXPECT_EQ(gtk::Get<5>(largeRankLast), 1);
  }

  {
    // Dimension Manipulation Tests (Enhanced with corner cases)
    // Test DimFront with various cases
    static_assert(DimFront<Vector> == 5);
    static_assert(DimFront<Matrix> == 3);
    static_assert(DimFront<SingleElementVector> == 1);
    static_assert(DimFront<LargeVector> == 1000);
    static_assert(DimFront<SingleRowMatrix> == 1);
    static_assert(DimFront<SingleColMatrix> == 10);
    static_assert(DimFront<MixedTensor> == 1);

    // Test DimPushFront with various cases
    using VectorPushed = DimPushFront<Vector, 7>;
    static_assert(std::is_same_v<VectorPushed, TDimension<7, 5>>);

    using MatrixPushed = DimPushFront<Matrix, 2>;
    static_assert(std::is_same_v<MatrixPushed, TDimension<2, 3, 4>>);

    // Corner cases for DimPushFront
    using ScalarPushed = DimPushFront<Scalar, 42>;
    static_assert(std::is_same_v<ScalarPushed, TDimension<42>>);

    using SingleElementPushed = DimPushFront<SingleElementVector, 99>;
    static_assert(std::is_same_v<SingleElementPushed, TDimension<99, 1>>);

    using LargePushed = DimPushFront<LargeRankTensor, 100>;
    static_assert(std::is_same_v<LargePushed, TDimension<100, 2, 2, 2, 2, 2, 2>>);

    // Test DimPopFront with various cases
    using MatrixPopped = DimPopFront<Matrix>;
    static_assert(std::is_same_v<MatrixPopped, TDimension<4>>);

    // Corner cases for DimPopFront
    using SingleElementPopped = DimPopFront<SingleElementVector>;
    static_assert(std::is_same_v<SingleElementPopped, TDimension<>>);  // Becomes scalar

    using SingleRowPopped = DimPopFront<SingleRowMatrix>;
    static_assert(std::is_same_v<SingleRowPopped, TDimension<10>>);

    using SingleColPopped = DimPopFront<SingleColMatrix>;
    static_assert(std::is_same_v<SingleColPopped, TDimension<1>>);

    using LargeRankPopped = DimPopFront<LargeRankTensor>;
    static_assert(std::is_same_v<LargeRankPopped, TDimension<2, 2, 2, 2, 2>>);

    // Test DimReverse with various cases
    using MatrixReversed = DimReverse<Matrix>;
    static_assert(std::is_same_v<MatrixReversed, TDimension<4, 3>>);

    using Tensor3DReversed = DimReverse<Tensor3D>;
    static_assert(std::is_same_v<Tensor3DReversed, TDimension<4, 3, 2>>);

    // Corner cases for DimReverse
    using ScalarReversed = DimReverse<Scalar>;
    static_assert(std::is_same_v<ScalarReversed, TDimension<>>);  // Scalar stays scalar

    using VectorReversed = DimReverse<Vector>;
    static_assert(std::is_same_v<VectorReversed, TDimension<5>>);  // Vector stays same

    using SingleElementReversed = DimReverse<SingleElementVector>;
    static_assert(std::is_same_v<SingleElementReversed, TDimension<1>>);  // Stays same

    using SingleRowReversed = DimReverse<SingleRowMatrix>;
    static_assert(std::is_same_v<SingleRowReversed, TDimension<10, 1>>);

    using SingleColReversed = DimReverse<SingleColMatrix>;
    static_assert(std::is_same_v<SingleColReversed, TDimension<1, 10>>);

    using SquareReversed = DimReverse<SquareMatrix>;
    static_assert(std::is_same_v<SquareReversed, TDimension<5, 5>>);  // Square stays same

    using MixedReversed = DimReverse<MixedTensor>;
    static_assert(std::is_same_v<MixedReversed, TDimension<3, 1, 5, 1>>);

    using LargeRankReversed = DimReverse<LargeRankTensor>;
    static_assert(std::is_same_v<LargeRankReversed, TDimension<2, 2, 2, 2, 2, 2>>
    );  // All same, stays same
  }

  {
    // Broadcasting Compatibility Tests
    // Additional types for specific broadcasting tests
    using BroadcastVector = TDimension<5>;
    using BroadcastMatrix = TDimension<3, 5>;
    using BroadcastTensor3D = TDimension<2, 3, 5>;

    // Note: IsCompatibleDim and BroadcastAlignedDim have been removed from the header
    // Only CanBroadcast utility remains for broadcasting compatibility

    // Padding Tests - PadDim utility is still available
    // Test padding to same rank
    using PadResult = PadDim<BroadcastVector, BroadcastTensor3D>;
    static_assert(std::is_same_v<PadResult::D1Padded, TDimension<1, 1, 5>>);
    static_assert(std::is_same_v<PadResult::D2Padded, TDimension<2, 3, 5>>);

    using PadResult2 = PadDim<BroadcastMatrix, BroadcastTensor3D>;
    static_assert(std::is_same_v<PadResult2::D1Padded, TDimension<1, 3, 5>>);
    static_assert(std::is_same_v<PadResult2::D2Padded, TDimension<2, 3, 5>>);

    // Test when dimensions have same rank
    using PadResultSame = PadDim<BroadcastMatrix, TDimension<4, 6>>;
    static_assert(std::is_same_v<PadResultSame::D1Padded, TDimension<3, 5>>);
    static_assert(std::is_same_v<PadResultSame::D2Padded, TDimension<4, 6>>);

    // Comprehensive PadDim Tests
    // Test that PadDim always pads the smaller dimension with leading ones, regardless of order

    // Case 1: Scalar (rank 0) vs higher rank dimensions
    using PadScalar1D = PadDim<Scalar, Vector>;  // 0D vs 1D
    static_assert(std::is_same_v<PadScalar1D::D1Padded, TDimension<1>>);     // Scalar padded to 1D with leading 1
    static_assert(std::is_same_v<PadScalar1D::D2Padded, TDimension<5>>);     // Vector unchanged

    using Pad1DScalar = PadDim<Vector, Scalar>;  // 1D vs 0D
    static_assert(std::is_same_v<Pad1DScalar::D1Padded, TDimension<5>>);     // Vector unchanged
    static_assert(std::is_same_v<Pad1DScalar::D2Padded, TDimension<1>>);     // Scalar padded to 1D with leading 1

    using PadScalar2D = PadDim<Scalar, Matrix>;  // 0D vs 2D
    static_assert(std::is_same_v<PadScalar2D::D1Padded, TDimension<1, 1>>);  // Scalar padded to 2D with ones
    static_assert(std::is_same_v<PadScalar2D::D2Padded, TDimension<3, 4>>);  // Matrix unchanged

    using Pad2DScalar = PadDim<Matrix, Scalar>;  // 2D vs 0D
    static_assert(std::is_same_v<Pad2DScalar::D1Padded, TDimension<3, 4>>);  // Matrix unchanged
    static_assert(std::is_same_v<Pad2DScalar::D2Padded, TDimension<1, 1>>);  // Scalar padded to 2D with ones

    using PadScalar3D = PadDim<Scalar, Tensor3D>;  // 0D vs 3D
    static_assert(std::is_same_v<PadScalar3D::D1Padded, TDimension<1, 1, 1>>);  // Scalar padded to 3D
    static_assert(std::is_same_v<PadScalar3D::D2Padded, TDimension<2, 3, 4>>);  // Tensor unchanged

    // Case 2: 1D vs higher rank dimensions
    using Pad1D2D = PadDim<Vector, Matrix>;  // 1D vs 2D
    static_assert(std::is_same_v<Pad1D2D::D1Padded, TDimension<1, 5>>);      // Vector padded with leading 1
    static_assert(std::is_same_v<Pad1D2D::D2Padded, TDimension<3, 4>>);      // Matrix unchanged

    using Pad2D1D = PadDim<Matrix, Vector>;  // 2D vs 1D
    static_assert(std::is_same_v<Pad2D1D::D1Padded, TDimension<3, 4>>);      // Matrix unchanged
    static_assert(std::is_same_v<Pad2D1D::D2Padded, TDimension<1, 5>>);      // Vector padded with leading 1

    using Pad1D3D = PadDim<Vector, Tensor3D>;  // 1D vs 3D
    static_assert(std::is_same_v<Pad1D3D::D1Padded, TDimension<1, 1, 5>>);   // Vector padded with two leading 1s
    static_assert(std::is_same_v<Pad1D3D::D2Padded, TDimension<2, 3, 4>>);   // Tensor unchanged

    using Pad3D1D = PadDim<Tensor3D, Vector>;  // 3D vs 1D
    static_assert(std::is_same_v<Pad3D1D::D1Padded, TDimension<2, 3, 4>>);   // Tensor unchanged
    static_assert(std::is_same_v<Pad3D1D::D2Padded, TDimension<1, 1, 5>>);   // Vector padded with two leading 1s

    using Pad1D4D = PadDim<Vector, Tensor4D>;  // 1D vs 4D
    static_assert(std::is_same_v<Pad1D4D::D1Padded, TDimension<1, 1, 1, 5>>);  // Vector padded with three leading 1s
    static_assert(std::is_same_v<Pad1D4D::D2Padded, TDimension<2, 3, 4, 5>>);  // 4D tensor unchanged

    // Case 3: 2D vs higher rank dimensions
    using Pad2D3D = PadDim<Matrix, Tensor3D>;  // 2D vs 3D
    static_assert(std::is_same_v<Pad2D3D::D1Padded, TDimension<1, 3, 4>>);   // Matrix padded with one leading 1
    static_assert(std::is_same_v<Pad2D3D::D2Padded, TDimension<2, 3, 4>>);   // Tensor unchanged

    using Pad3D2D = PadDim<Tensor3D, Matrix>;  // 3D vs 2D
    static_assert(std::is_same_v<Pad3D2D::D1Padded, TDimension<2, 3, 4>>);   // Tensor unchanged
    static_assert(std::is_same_v<Pad3D2D::D2Padded, TDimension<1, 3, 4>>);   // Matrix padded with one leading 1

    using Pad2D4D = PadDim<Matrix, Tensor4D>;  // 2D vs 4D
    static_assert(std::is_same_v<Pad2D4D::D1Padded, TDimension<1, 1, 3, 4>>);  // Matrix padded with two leading 1s
    static_assert(std::is_same_v<Pad2D4D::D2Padded, TDimension<2, 3, 4, 5>>);  // 4D tensor unchanged

    // Case 4: 3D vs 4D
    using Pad3D4D = PadDim<Tensor3D, Tensor4D>;  // 3D vs 4D
    static_assert(std::is_same_v<Pad3D4D::D1Padded, TDimension<1, 2, 3, 4>>);  // 3D padded with one leading 1
    static_assert(std::is_same_v<Pad3D4D::D2Padded, TDimension<2, 3, 4, 5>>);  // 4D unchanged

    using Pad4D3D = PadDim<Tensor4D, Tensor3D>;  // 4D vs 3D
    static_assert(std::is_same_v<Pad4D3D::D1Padded, TDimension<2, 3, 4, 5>>);  // 4D unchanged
    static_assert(std::is_same_v<Pad4D3D::D2Padded, TDimension<1, 2, 3, 4>>);  // 3D padded with one leading 1

    // Case 5: Same rank dimensions (should remain unchanged)
    using PadSameScalar = PadDim<Scalar, TDimension<>>;  // Both scalar
    static_assert(std::is_same_v<PadSameScalar::D1Padded, TDimension<>>);
    static_assert(std::is_same_v<PadSameScalar::D2Padded, TDimension<>>);

    using PadSame1D = PadDim<Vector, TDimension<10>>;  // Both 1D
    static_assert(std::is_same_v<PadSame1D::D1Padded, TDimension<5>>);
    static_assert(std::is_same_v<PadSame1D::D2Padded, TDimension<10>>);

    using PadSame2D = PadDim<Matrix, TDimension<7, 8>>;  // Both 2D
    static_assert(std::is_same_v<PadSame2D::D1Padded, TDimension<3, 4>>);
    static_assert(std::is_same_v<PadSame2D::D2Padded, TDimension<7, 8>>);

    using PadSame3D = PadDim<Tensor3D, TDimension<5, 6, 7>>;  // Both 3D
    static_assert(std::is_same_v<PadSame3D::D1Padded, TDimension<2, 3, 4>>);
    static_assert(std::is_same_v<PadSame3D::D2Padded, TDimension<5, 6, 7>>);

    // Case 6: Edge cases with dimensions containing 1s
    using SingleDim1D = TDimension<1>;
    using PadSingle2D = PadDim<SingleDim1D, Matrix>;  // 1D [1] vs 2D [3,4]
    static_assert(std::is_same_v<PadSingle2D::D1Padded, TDimension<1, 1>>);   // [1] padded to [1,1]
    static_assert(std::is_same_v<PadSingle2D::D2Padded, TDimension<3, 4>>);   // [3,4] unchanged

    using SingleRowMat = TDimension<1, 5>;
    using PadSingleRow3D = PadDim<SingleRowMat, Tensor3D>;  // 2D [1,5] vs 3D [2,3,4]
    static_assert(std::is_same_v<PadSingleRow3D::D1Padded, TDimension<1, 1, 5>>);  // [1,5] padded to [1,1,5]
    static_assert(std::is_same_v<PadSingleRow3D::D2Padded, TDimension<2, 3, 4>>);  // [2,3,4] unchanged

    using MixedDim = TDimension<1, 3, 1>;
    using PadMixed4D = PadDim<MixedDim, Tensor4D>;  // 3D [1,3,1] vs 4D [2,3,4,5]
    static_assert(std::is_same_v<PadMixed4D::D1Padded, TDimension<1, 1, 3, 1>>);   // [1,3,1] padded to [1,1,3,1]
    static_assert(std::is_same_v<PadMixed4D::D2Padded, TDimension<2, 3, 4, 5>>);   // [2,3,4,5] unchanged

    // Case 7: Large rank differences
    using Large6D = TDimension<2, 2, 2, 2, 2, 2>;
    using PadScalar6D = PadDim<Scalar, Large6D>;  // 0D vs 6D
    static_assert(std::is_same_v<PadScalar6D::D1Padded, TDimension<1, 1, 1, 1, 1, 1>>);  // Scalar padded to 6D with all 1s
    static_assert(std::is_same_v<PadScalar6D::D2Padded, TDimension<2, 2, 2, 2, 2, 2>>);  // 6D unchanged

    using Pad1D6D = PadDim<Vector, Large6D>;  // 1D vs 6D
    static_assert(std::is_same_v<Pad1D6D::D1Padded, TDimension<1, 1, 1, 1, 1, 5>>);     // 1D padded with 5 leading 1s
    static_assert(std::is_same_v<Pad1D6D::D2Padded, TDimension<2, 2, 2, 2, 2, 2>>);     // 6D unchanged

    // Test CanBroadcast (now with rank constraint: FromDim::rank <= ToDim::rank)
    static_assert(CanBroadcast<TDimension<5>, TDimension<3, 5>>::value);     // 1D -> 2D: valid (5 matches last dim)
    static_assert(CanBroadcast<TDimension<1>, TDimension<3, 5>>::value);     // 1D -> 2D: valid (1 can broadcast)
    static_assert(CanBroadcast<TDimension<3, 1>, TDimension<3, 5>>::value);  // 2D -> 2D: valid (3==3, 1->5)
    static_assert(!CanBroadcast<TDimension<4>, TDimension<3, 5>>::value);    // 1D -> 2D: invalid (4 != 5 and 4 != 1)
  }

  {
    // Dimension Removal Tests
    // Test DimPopFront - removes the first dimension
    using VectorPopped = DimPopFront<Vector>;
    static_assert(std::is_same_v<VectorPopped, TDimension<>>);  // Should become empty/scalar

    using MatrixPoppedFront = DimPopFront<Matrix>;
    static_assert(std::is_same_v<MatrixPoppedFront, TDimension<4>>);

    using Tensor3DPopped = DimPopFront<Tensor3D>;
    static_assert(std::is_same_v<Tensor3DPopped, TDimension<3, 4>>);

    using Tensor4DPopped = DimPopFront<Tensor4D>;
    static_assert(std::is_same_v<Tensor4DPopped, TDimension<3, 4, 5>>);

    // Test multiple pops
    using MatrixDoublePopped = DimPopFront<MatrixPoppedFront>;
    static_assert(std::is_same_v<MatrixDoublePopped, TDimension<>>);

    using Tensor3DDoublePopped = DimPopFront<Tensor3DPopped>;
    static_assert(std::is_same_v<Tensor3DDoublePopped, TDimension<4>>);

    using Tensor4DDoublePopped = DimPopFront<Tensor4DPopped>;
    static_assert(std::is_same_v<Tensor4DDoublePopped, TDimension<4, 5>>);

    // Test DimPopBack - removes the last dimension
    using VectorPoppedBack = DimPopBack<Vector>;
    static_assert(std::is_same_v<VectorPoppedBack, TDimension<>>);  // Should become empty/scalar

    using MatrixPoppedBack = DimPopBack<Matrix>;
    static_assert(std::is_same_v<MatrixPoppedBack, TDimension<3>>);

    using Tensor3DPoppedBack = DimPopBack<Tensor3D>;
    static_assert(std::is_same_v<Tensor3DPoppedBack, TDimension<2, 3>>);

    using Tensor4DPoppedBack = DimPopBack<Tensor4D>;
    static_assert(std::is_same_v<Tensor4DPoppedBack, TDimension<2, 3, 4>>);

    // Test combining front and back pops
    using MatrixPopFrontThenBack = DimPopBack<MatrixPoppedFront>;
    static_assert(std::is_same_v<MatrixPopFrontThenBack, TDimension<>>);

    using MatrixPopBackThenFront = DimPopFront<MatrixPoppedBack>;
    static_assert(std::is_same_v<MatrixPopBackThenFront, TDimension<>>);

    using Tensor3DPopFrontThenBack = DimPopBack<Tensor3DPopped>;
    static_assert(std::is_same_v<Tensor3DPopFrontThenBack, TDimension<3>>);

    using Tensor3DPopBackThenFront = DimPopFront<Tensor3DPoppedBack>;
    static_assert(std::is_same_v<Tensor3DPopBackThenFront, TDimension<3>>);

    // Test edge cases with single dimension
    using SingleDim = TDimension<7>;
    using SingleDimPoppedBack = DimPopBack<SingleDim>;
    using SingleDimPoppedFront = DimPopFront<SingleDim>;
    static_assert(std::is_same_v<SingleDimPoppedBack, TDimension<>>);
    static_assert(std::is_same_v<SingleDimPoppedFront, TDimension<>>);

    // Test symmetry: pop front and back should give same result for vectors
    static_assert(std::is_same_v<SingleDimPoppedBack, SingleDimPoppedFront>);

    // Test with different dimension patterns
    using SymmetricTensor = TDimension<3, 3, 3>;
    using SymmetricTensorPoppedBack = DimPopBack<SymmetricTensor>;
    using SymmetricTensorPoppedFront = DimPopFront<SymmetricTensor>;
    static_assert(std::is_same_v<SymmetricTensorPoppedBack, TDimension<3, 3>>);
    static_assert(std::is_same_v<SymmetricTensorPoppedFront, TDimension<3, 3>>);

    // These should be the same since all dimensions are equal
    static_assert(std::is_same_v<SymmetricTensorPoppedBack, SymmetricTensorPoppedFront>);

    // Test with asymmetric dimensions
    using AsymmetricTensor = TDimension<2, 5, 7>;
    using AsymmetricTensorPoppedBack = DimPopBack<AsymmetricTensor>;
    using AsymmetricTensorPoppedFront = DimPopFront<AsymmetricTensor>;
    static_assert(std::is_same_v<AsymmetricTensorPoppedBack, TDimension<2, 5>>);
    static_assert(std::is_same_v<AsymmetricTensorPoppedFront, TDimension<5, 7>>);

    // These should be different since dimensions are different
    static_assert(!std::is_same_v<AsymmetricTensorPoppedBack, AsymmetricTensorPoppedFront>);

    // Test various rank and count properties after operations
    static_assert(VectorPoppedBack::rank == 0);  // TDimension<> has rank 0 (true scalar)
    static_assert(MatrixPoppedBack::rank == 1);
    static_assert(Tensor3DPoppedBack::rank == 2);
    static_assert(Tensor4DPoppedBack::rank == 3);

    static_assert(VectorPoppedBack::count == 1);     // Scalar has count 1
    static_assert(MatrixPoppedBack::count == 3);     // TDimension<3> has count 3
    static_assert(Tensor3DPoppedBack::count == 6);   // TDimension<2, 3> has count 6
    static_assert(Tensor4DPoppedBack::count == 24);  // TDimension<2, 3, 4> has count 24
  }

  {
    // Element Removal Tests (Enhanced with corner cases)
    // Test DimRemoveElement
    using MatrixRemove3 = typename DimRemoveElement<Matrix, 3>::Type;
    static_assert(std::is_same_v<MatrixRemove3, TDimension<4>>);

    using Tensor3DRemove3 = typename DimRemoveElement<Tensor3D, 3>::Type;
    static_assert(std::is_same_v<Tensor3DRemove3, TDimension<2, 4>>);

    using Tensor4DRemove4 = typename DimRemoveElement<Tensor4D, 4>::Type;
    static_assert(std::is_same_v<Tensor4DRemove4, TDimension<2, 3, 5>>);

    // Corner cases for DimRemoveElement
    using VectorRemove5 = typename DimRemoveElement<Vector, 5>::Type;
    static_assert(std::is_same_v<VectorRemove5, TDimension<>>);  // Becomes scalar

    using SingleElementRemove1 = typename DimRemoveElement<SingleElementVector, 1>::Type;
    static_assert(std::is_same_v<SingleElementRemove1, TDimension<>>);  // Becomes scalar

    using SingleRowRemove1 = typename DimRemoveElement<SingleRowMatrix, 1>::Type;
    static_assert(std::is_same_v<SingleRowRemove1, TDimension<10>>);  // Becomes vector

    using SingleRowRemove10 = typename DimRemoveElement<SingleRowMatrix, 10>::Type;
    static_assert(std::is_same_v<SingleRowRemove10, TDimension<1>>);  // Becomes vector

    using SquareRemove5 = typename DimRemoveElement<SquareMatrix, 5>::Type;
    static_assert(std::is_same_v<SquareRemove5, TDimension<>>);  // Both 5s removed, becomes scalar

    using MixedRemove1 = typename DimRemoveElement<MixedTensor, 1>::Type;
    static_assert(std::is_same_v<MixedRemove1, TDimension<5, 3>>);  // Remove both 1s

    using MixedRemove5 = typename DimRemoveElement<MixedTensor, 5>::Type;
    static_assert(std::is_same_v<MixedRemove5, TDimension<1, 1, 3>>);  // Remove 5

    using LargeRankRemove2 = typename DimRemoveElement<LargeRankTensor, 2>::Type;
    static_assert(std::is_same_v<LargeRankRemove2, TDimension<>>
    );  // All 2s removed, becomes scalar
  }

  {
    // Broadcasting Tests (Enhanced with corner cases)
    // Note: BroadcastAlignedDim, IsCompatibleDim, and BroadcastDimT have been removed from header

    // Test CanBroadcast (now with rank constraint: FromDim::rank <= ToDim::rank)
    // Basic valid broadcasting cases (rank constraint satisfied)
    static_assert(CanBroadcast<TDimension<5>, TDimension<3, 5>>::value);     // 1D -> 2D: valid (5 matches last dim)
    static_assert(CanBroadcast<TDimension<1>, TDimension<3, 5>>::value);     // 1D -> 2D: valid (1 can broadcast)
    static_assert(CanBroadcast<TDimension<3, 1>, TDimension<3, 5>>::value);  // 2D -> 2D: valid (3==3, 1->5)
    static_assert(!CanBroadcast<TDimension<4>, TDimension<3, 5>>::value);    // 1D -> 2D: invalid (4 != 5 and 4 != 1)

    // Test rank constraint: FromDim::rank <= ToDim::rank (valid cases)
    static_assert(CanBroadcast<Scalar, Vector>::value);                      // 0D -> 1D: valid
    static_assert(CanBroadcast<Scalar, Matrix>::value);                      // 0D -> 2D: valid
    static_assert(CanBroadcast<Scalar, Tensor3D>::value);                    // 0D -> 3D: valid
    
    // 1D -> 2D: valid only if the single dimension matches the last dimension of target
    static_assert(CanBroadcast<TDimension<4>, TDimension<3, 4>>::value);     // 1D -> 2D: valid (4 matches last dim)
    static_assert(CanBroadcast<TDimension<1>, TDimension<3, 4>>::value);     // 1D -> 2D: valid (1 can broadcast)
    static_assert(!CanBroadcast<TDimension<5>, TDimension<3, 4>>::value);    // 1D -> 2D: invalid (5 != 4)
    
    // 1D -> 3D: valid only if dimensions match appropriately  
    static_assert(CanBroadcast<TDimension<4>, TDimension<2, 3, 4>>::value);  // 1D -> 3D: valid (4 matches last dim)
    static_assert(CanBroadcast<TDimension<1>, TDimension<2, 3, 4>>::value);  // 1D -> 3D: valid (1 can broadcast)
    static_assert(!CanBroadcast<TDimension<5>, TDimension<2, 3, 4>>::value); // 1D -> 3D: invalid (5 != 4)
    
    // 2D -> 3D: valid only if last 2 dimensions match
    static_assert(CanBroadcast<TDimension<3, 4>, TDimension<2, 3, 4>>::value); // 2D -> 3D: valid (3,4 match last dims)
    static_assert(CanBroadcast<TDimension<1, 4>, TDimension<2, 3, 4>>::value); // 2D -> 3D: valid (1->3, 4==4)
    static_assert(!CanBroadcast<TDimension<3, 5>, TDimension<2, 3, 4>>::value); // 2D -> 3D: invalid (5 != 4)

    // Test rank constraint violations: FromDim::rank > ToDim::rank 
    // These should now compile and return false (thanks to SFINAE-based implementation)
    static_assert(!CanBroadcast<Vector, Scalar>::value);                     // 1D -> 0D: invalid (rank constraint)
    static_assert(!CanBroadcast<Matrix, Scalar>::value);                     // 2D -> 0D: invalid (rank constraint)
    static_assert(!CanBroadcast<Matrix, Vector>::value);                     // 2D -> 1D: invalid (rank constraint)
    static_assert(!CanBroadcast<Tensor3D, Scalar>::value);                   // 3D -> 0D: invalid (rank constraint)
    static_assert(!CanBroadcast<Tensor3D, Vector>::value);                   // 3D -> 1D: invalid (rank constraint)
    static_assert(!CanBroadcast<Tensor3D, Matrix>::value);                   // 3D -> 2D: invalid (rank constraint)
    static_assert(!CanBroadcast<LargeRankTensor, Vector>::value);            // 6D -> 1D: invalid (rank constraint)
    static_assert(!CanBroadcast<LargeRankTensor, Matrix>::value);            // 6D -> 2D: invalid (rank constraint)
    static_assert(!CanBroadcast<LargeRankTensor, Tensor3D>::value);          // 6D -> 3D: invalid (rank constraint)

    // Corner cases for CanBroadcast with rank constraint satisfied
    static_assert(CanBroadcast<Scalar, LargeRankTensor>::value);             // 0D -> 6D: valid
    static_assert(CanBroadcast<SingleElementVector, Vector>::value);         // 1D -> 1D: valid (TDimension<1> to TDimension<5>)
    static_assert(CanBroadcast<SingleElementVector, Matrix>::value);         // 1D -> 2D: valid (TDimension<1> can broadcast)
    
    // Same-rank broadcasting cases
    static_assert(!CanBroadcast<Vector, SingleElementVector>::value);        // 1D -> 1D: invalid (5 != 1 and 5 != 1)

    static_assert(CanBroadcast<TDimension<1, 1>, Matrix>::value);            // 2D -> 2D: valid (TDimension<1,1> can broadcast to any matrix)
    static_assert(CanBroadcast<TDimension<1, 1, 1>, Tensor3D>::value);      // 3D -> 3D: valid (TDimension<1,1,1> can broadcast to any 3D tensor)

    // Test same-rank broadcasting with dimension compatibility
    static_assert(CanBroadcast<TDimension<1, 4>, TDimension<3, 4>>::value);  // 2D -> 2D: valid (1 can broadcast to 3, 4 == 4)
    static_assert(CanBroadcast<TDimension<3, 1>, TDimension<3, 5>>::value);  // 2D -> 2D: valid (3 == 3, 1 can broadcast to 5)
    static_assert(!CanBroadcast<TDimension<2, 4>, TDimension<3, 4>>::value); // 2D -> 2D: invalid (2 != 3 and 2 != 1)

    // Test mixed dimensions containing 1s with rank constraint
    static_assert(CanBroadcast<MixedTensor, TDimension<2, 5, 3, 3>>::value); // 4D -> 4D: valid (TDimension<1,5,1,3> can broadcast to TDimension<2,5,3,3>)
    static_assert(!CanBroadcast<MixedTensor, TDimension<2, 4, 3, 3>>::value); // 4D -> 4D: invalid (5 != 4, cannot broadcast)

    // Test lower-rank to higher-rank broadcasting
    static_assert(CanBroadcast<TDimension<5>, TDimension<1, 1, 5>>::value);   // 1D -> 3D: valid (5 compatible with last dim)
    static_assert(CanBroadcast<TDimension<1>, TDimension<4, 3, 1>>::value);   // 1D -> 3D: valid (1 can broadcast to anything)
    static_assert(CanBroadcast<TDimension<3, 4>, TDimension<2, 3, 4>>::value); // 2D -> 3D: valid (3,4 compatible with last dims)
    
    // Test cases that should fail due to dimension incompatibility (even with rank constraint satisfied)
    static_assert(!CanBroadcast<TDimension<5>, TDimension<1, 1, 4>>::value);  // 1D -> 3D: invalid (5 != 4)
    static_assert(!CanBroadcast<TDimension<2, 3>, TDimension<1, 4, 3>>::value); // 2D -> 3D: invalid (2 != 4)

    // Additional tests to verify the SFINAE fix works correctly
    // These would previously cause compilation errors, now they should cleanly return false
    static_assert(!CanBroadcast<TDimension<2, 3, 4>, TDimension<5>>::value);          // 3D -> 1D: invalid
    static_assert(!CanBroadcast<TDimension<2, 3, 4, 5>, TDimension<1, 2>>::value);   // 4D -> 2D: invalid
    static_assert(!CanBroadcast<LargeRankTensor, TDimension<1>>::value);              // 6D -> 1D: invalid
    static_assert(!CanBroadcast<MixedTensor, TDimension<1, 1>>::value);               // 4D -> 2D: invalid
    
    // Test extreme rank differences
    static_assert(!CanBroadcast<LargeRankTensor, Scalar>::value);                     // 6D -> 0D: invalid
    static_assert(CanBroadcast<Scalar, LargeRankTensor>::value);                      // 0D -> 6D: valid
  }

  {
    // Advanced Pop Operations Tests (Enhanced with corner cases)
    // Test DimPopFrontAndBack - now fixed
    using PopBothResult0 = typename DimPopFrontAndBack<Matrix, 0, 0>::Type;
    static_assert(std::is_same_v<PopBothResult0, Matrix>);

    // Test popping only from front
    using PopFront1 =
      typename DimPopFrontAndBack<Tensor3D, 1, 0>::Type;  // TDimension<2,3,4> -> TDimension<3,4>
    static_assert(std::is_same_v<PopFront1, TDimension<3, 4>>);

    // Test popping only from back
    using PopBack1 =
      typename DimPopFrontAndBack<Tensor3D, 0, 1>::Type;  // TDimension<2,3,4> -> TDimension<2,3>
    static_assert(std::is_same_v<PopBack1, TDimension<2, 3>>);

    // Test popping from both front and back
    using PopBoth1 =
      typename DimPopFrontAndBack<Tensor4D, 1, 1>::Type;  // TDimension<2,3,4,5> -> TDimension<3,4>
    static_assert(std::is_same_v<PopBoth1, TDimension<3, 4>>);

    // Corner cases for DimPopFrontAndBack
    using PopScalar = typename DimPopFrontAndBack<Scalar, 0, 0>::Type;
    static_assert(std::is_same_v<PopScalar, Scalar>);  // Scalar unchanged

    using PopVectorFront = typename DimPopFrontAndBack<Vector, 1, 0>::Type;
    static_assert(std::is_same_v<PopVectorFront, TDimension<>>);  // Becomes scalar

    using PopVectorBack = typename DimPopFrontAndBack<Vector, 0, 1>::Type;
    static_assert(std::is_same_v<PopVectorBack, TDimension<>>);  // Becomes scalar

    using PopMatrixBoth = typename DimPopFrontAndBack<Matrix, 1, 1>::Type;
    static_assert(std::is_same_v<PopMatrixBoth, TDimension<>>);  // Becomes scalar

    using PopSingleRowFront = typename DimPopFrontAndBack<SingleRowMatrix, 1, 0>::Type;
    static_assert(std::is_same_v<PopSingleRowFront, TDimension<10>>);  // Becomes vector

    using PopSingleColBack = typename DimPopFrontAndBack<SingleColMatrix, 0, 1>::Type;
    static_assert(std::is_same_v<PopSingleColBack, TDimension<10>>);  // Becomes vector

    using PopMixedBoth = typename DimPopFrontAndBack<MixedTensor, 1, 1>::Type;
    static_assert(std::is_same_v<PopMixedBoth, TDimension<5, 1>>
    );  // TDimension<1,5,1,3> -> TDimension<5,1>

    using PopLargeRankBoth = typename DimPopFrontAndBack<LargeRankTensor, 2, 2>::Type;
    static_assert(std::is_same_v<PopLargeRankBoth, TDimension<2, 2>>
    );  // TDimension<2,2,2,2,2,2> -> TDimension<2,2>

    // Test extreme cases
    using PopTensor3DAll = typename DimPopFrontAndBack<Tensor3D, 2, 1>::Type;
    static_assert(std::is_same_v<PopTensor3DAll, TDimension<>>);  // Pop all dimensions

    using PopTensor4DMost = typename DimPopFrontAndBack<Tensor4D, 2, 1>::Type;
    static_assert(std::is_same_v<PopTensor4DMost, TDimension<4>>
    );  // TDimension<2,3,4,5> -> TDimension<4>
  }

  {
    // Runtime Utilities Tests (Enhanced with corner cases)
    // Test TailProducts (runtime functionality)
    constexpr auto matrixTailProducts = Matrix::TailProducts();
    static_assert(matrixTailProducts[0] == 4);  // For TDimension<3, 4>: [4, 1]
    static_assert(matrixTailProducts[1] == 1);

    constexpr auto tensorTailProducts = Tensor3D::TailProducts();
    static_assert(tensorTailProducts[0] == 12);  // For TDimension<2, 3, 4>: [12, 4, 1]
    static_assert(tensorTailProducts[1] == 4);
    static_assert(tensorTailProducts[2] == 1);

    // Corner cases for TailProducts
    // Note: Scalar TailProducts may not be supported or return void
    // constexpr auto scalarTailProducts = Scalar::TailProducts();
    // Scalar has no dimensions, so TailProducts may not be applicable

    constexpr auto vectorTailProducts = Vector::TailProducts();
    static_assert(vectorTailProducts[0] == 1);  // For TDimension<5>: [1]

    constexpr auto singleElementTailProducts = SingleElementVector::TailProducts();
    static_assert(singleElementTailProducts[0] == 1);  // For TDimension<1>: [1]

    constexpr auto singleRowTailProducts = SingleRowMatrix::TailProducts();
    static_assert(singleRowTailProducts[0] == 10);  // For TDimension<1, 10>: [10, 1]
    static_assert(singleRowTailProducts[1] == 1);

    constexpr auto singleColTailProducts = SingleColMatrix::TailProducts();
    static_assert(singleColTailProducts[0] == 1);  // For TDimension<10, 1>: [1, 1]
    static_assert(singleColTailProducts[1] == 1);

    constexpr auto squareTailProducts = SquareMatrix::TailProducts();
    static_assert(squareTailProducts[0] == 5);  // For TDimension<5, 5>: [5, 1]
    static_assert(squareTailProducts[1] == 1);

    constexpr auto mixedTailProducts = MixedTensor::TailProducts();
    static_assert(mixedTailProducts[0] == 15);  // For TDimension<1, 5, 1, 3>: [15, 3, 3, 1]
    static_assert(mixedTailProducts[1] == 3);
    static_assert(mixedTailProducts[2] == 3);
    static_assert(mixedTailProducts[3] == 1);

    constexpr auto largeRankTailProducts = LargeRankTensor::TailProducts();
    static_assert(
      largeRankTailProducts[0] == 32
    );  // For TDimension<2,2,2,2,2,2>: [32, 16, 8, 4, 2, 1]
    static_assert(largeRankTailProducts[1] == 16);
    static_assert(largeRankTailProducts[2] == 8);
    static_assert(largeRankTailProducts[3] == 4);
    static_assert(largeRankTailProducts[4] == 2);
    static_assert(largeRankTailProducts[5] == 1);

    constexpr auto tensor4DTailProducts = Tensor4D::TailProducts();
    static_assert(tensor4DTailProducts[0] == 60);  // For TDimension<2,3,4,5>: [60, 20, 5, 1]
    static_assert(tensor4DTailProducts[1] == 20);
    static_assert(tensor4DTailProducts[2] == 5);
    static_assert(tensor4DTailProducts[3] == 1);
  }

  {
    // DimExtractFront Tests - Extract first N dimensions
    using ExtractFront1FromMatrix = typename DimExtractFront<Matrix, 1>::Type;
    static_assert(std::is_same_v<ExtractFront1FromMatrix, TDimension<3>>);

    using ExtractFront2FromTensor3D = typename DimExtractFront<Tensor3D, 2>::Type;
    static_assert(std::is_same_v<ExtractFront2FromTensor3D, TDimension<2, 3>>);

    using ExtractFront3FromTensor4D = typename DimExtractFront<Tensor4D, 3>::Type;
    static_assert(std::is_same_v<ExtractFront3FromTensor4D, TDimension<2, 3, 4>>);

    // Corner cases for DimExtractFront
    using ExtractFront0FromVector = typename DimExtractFront<Vector, 0>::Type;
    static_assert(std::is_same_v<ExtractFront0FromVector, TDimension<>>);  // Empty dimension

    using ExtractFront1FromVector = typename DimExtractFront<Vector, 1>::Type;
    static_assert(std::is_same_v<ExtractFront1FromVector, TDimension<5>>);  // Entire vector

    using ExtractFront2FromMatrix = typename DimExtractFront<Matrix, 2>::Type;
    static_assert(std::is_same_v<ExtractFront2FromMatrix, TDimension<3, 4>>);  // Entire matrix

    using ExtractFront1FromSingleElement = typename DimExtractFront<SingleElementVector, 1>::Type;
    static_assert(std::is_same_v<ExtractFront1FromSingleElement, TDimension<1>>);

    using ExtractFront1FromSingleRow = typename DimExtractFront<SingleRowMatrix, 1>::Type;
    static_assert(std::is_same_v<ExtractFront1FromSingleRow, TDimension<1>>);

    using ExtractFront2FromMixed = typename DimExtractFront<MixedTensor, 2>::Type;
    static_assert(std::is_same_v<ExtractFront2FromMixed, TDimension<1, 5>>);

    using ExtractFront4FromLargeRank = typename DimExtractFront<LargeRankTensor, 4>::Type;
    static_assert(std::is_same_v<ExtractFront4FromLargeRank, TDimension<2, 2, 2, 2>>);
  }

  {
    // DimExtractBack Tests - Extract last N dimensions
    using ExtractBack1FromMatrix = typename DimExtractBack<Matrix, 1>::Type;
    static_assert(std::is_same_v<ExtractBack1FromMatrix, TDimension<4>>);

    using ExtractBack2FromTensor3D = typename DimExtractBack<Tensor3D, 2>::Type;
    static_assert(std::is_same_v<ExtractBack2FromTensor3D, TDimension<3, 4>>);

    using ExtractBack3FromTensor4D = typename DimExtractBack<Tensor4D, 3>::Type;
    static_assert(std::is_same_v<ExtractBack3FromTensor4D, TDimension<3, 4, 5>>);

    // Corner cases for DimExtractBack
    using ExtractBack0FromVector = typename DimExtractBack<Vector, 0>::Type;
    static_assert(std::is_same_v<ExtractBack0FromVector, TDimension<>>);  // Empty dimension

    using ExtractBack1FromVector = typename DimExtractBack<Vector, 1>::Type;
    static_assert(std::is_same_v<ExtractBack1FromVector, TDimension<5>>);  // Entire vector

    using ExtractBack2FromMatrix = typename DimExtractBack<Matrix, 2>::Type;
    static_assert(std::is_same_v<ExtractBack2FromMatrix, TDimension<3, 4>>);  // Entire matrix

    using ExtractBack1FromSingleElement = typename DimExtractBack<SingleElementVector, 1>::Type;
    static_assert(std::is_same_v<ExtractBack1FromSingleElement, TDimension<1>>);

    using ExtractBack1FromSingleCol = typename DimExtractBack<SingleColMatrix, 1>::Type;
    static_assert(std::is_same_v<ExtractBack1FromSingleCol, TDimension<1>>);

    using ExtractBack2FromMixed = typename DimExtractBack<MixedTensor, 2>::Type;
    static_assert(std::is_same_v<ExtractBack2FromMixed, TDimension<1, 3>>);

    using ExtractBack4FromLargeRank = typename DimExtractBack<LargeRankTensor, 4>::Type;
    static_assert(std::is_same_v<ExtractBack4FromLargeRank, TDimension<2, 2, 2, 2>>);
  }

  {
    // DimConcat Tests - Concatenate two dimensions
    using ConcatVectorVector = typename DimConcat<Vector, TDimension<7>>::Type;
    static_assert(std::is_same_v<ConcatVectorVector, TDimension<5, 7>>);

    using ConcatMatrixVector = typename DimConcat<Matrix, Vector>::Type;
    static_assert(std::is_same_v<ConcatMatrixVector, TDimension<3, 4, 5>>);

    using ConcatVectorMatrix = typename DimConcat<Vector, Matrix>::Type;
    static_assert(std::is_same_v<ConcatVectorMatrix, TDimension<5, 3, 4>>);

    using ConcatMatrixMatrix = typename DimConcat<Matrix, SquareMatrix>::Type;
    static_assert(std::is_same_v<ConcatMatrixMatrix, TDimension<3, 4, 5, 5>>);

    // Corner cases for DimConcat
    using ConcatScalarVector = typename DimConcat<Scalar, Vector>::Type;
    static_assert(std::is_same_v<ConcatScalarVector, TDimension<5>>);  // Scalar + Vector = Vector

    using ConcatVectorScalar = typename DimConcat<Vector, Scalar>::Type;
    static_assert(std::is_same_v<ConcatVectorScalar, TDimension<5>>);  // Vector + Scalar = Vector

    using ConcatScalarScalar = typename DimConcat<Scalar, Scalar>::Type;
    static_assert(std::is_same_v<ConcatScalarScalar, TDimension<>>);  // Scalar + Scalar = Scalar

    using ConcatSingleElements = typename DimConcat<SingleElementVector, SingleElementVector>::Type;
    static_assert(std::is_same_v<ConcatSingleElements, TDimension<1, 1>>);

    using ConcatMixedTensor3D = typename DimConcat<MixedTensor, Tensor3D>::Type;
    static_assert(std::is_same_v<ConcatMixedTensor3D, TDimension<1, 5, 1, 3, 2, 3, 4>>);

    using ConcatLargeRankTensor = typename DimConcat<LargeRankTensor, TDimension<9, 8>>::Type;
    static_assert(std::is_same_v<ConcatLargeRankTensor, TDimension<2, 2, 2, 2, 2, 2, 9, 8>>);
  }

  {
    // SubDim Tests - Extract a sub-dimension slice
    using SubDim1_1FromMatrix = typename SubDim<Matrix, 0, 1>::Type;
    static_assert(std::is_same_v<SubDim1_1FromMatrix, TDimension<3>>);  // First dimension

    using SubDim1_1FromMatrixEnd = typename SubDim<Matrix, 1, 1>::Type;
    static_assert(std::is_same_v<SubDim1_1FromMatrixEnd, TDimension<4>>);  // Second dimension

    using SubDim2_2FromTensor3D = typename SubDim<Tensor3D, 1, 2>::Type;
    static_assert(std::is_same_v<SubDim2_2FromTensor3D, TDimension<3, 4>>);  // Middle dimensions

    using SubDim1_2FromTensor4D = typename SubDim<Tensor4D, 1, 2>::Type;
    static_assert(std::is_same_v<SubDim1_2FromTensor4D, TDimension<3, 4>>);  // Middle dimensions

    using SubDim2_2FromTensor4D = typename SubDim<Tensor4D, 2, 2>::Type;
    static_assert(std::is_same_v<SubDim2_2FromTensor4D, TDimension<4, 5>>);  // Last two dimensions

    // Corner cases for SubDim
    using SubDim0_0FromVector = typename SubDim<Vector, 0, 0>::Type;
    static_assert(std::is_same_v<SubDim0_0FromVector, TDimension<>>);  // Empty slice

    using SubDim0_1FromVector = typename SubDim<Vector, 0, 1>::Type;
    static_assert(std::is_same_v<SubDim0_1FromVector, TDimension<5>>);  // Entire vector

    using SubDim0_2FromMatrix = typename SubDim<Matrix, 0, 2>::Type;
    static_assert(std::is_same_v<SubDim0_2FromMatrix, TDimension<3, 4>>);  // Entire matrix

    using SubDim0_0FromScalar = typename SubDim<Scalar, 0, 0>::Type;
    static_assert(std::is_same_v<SubDim0_0FromScalar, TDimension<>>);  // Empty from scalar

    using SubDim1_1FromSingleRow = typename SubDim<SingleRowMatrix, 1, 1>::Type;
    static_assert(std::is_same_v<SubDim1_1FromSingleRow, TDimension<10>>);  // Second dimension only

    using SubDim0_1FromSingleCol = typename SubDim<SingleColMatrix, 0, 1>::Type;
    static_assert(std::is_same_v<SubDim0_1FromSingleCol, TDimension<10>>);  // First dimension only

    using SubDim1_2FromMixed = typename SubDim<MixedTensor, 1, 2>::Type;
    static_assert(std::is_same_v<SubDim1_2FromMixed, TDimension<5, 1>>);  // Middle slice

    using SubDim2_2FromLargeRank = typename SubDim<LargeRankTensor, 2, 2>::Type;
    static_assert(std::is_same_v<SubDim2_2FromLargeRank, TDimension<2, 2>>);  // Middle two dimensions

    using SubDim1_4FromLargeRank = typename SubDim<LargeRankTensor, 1, 4>::Type;
    static_assert(std::is_same_v<SubDim1_4FromLargeRank, TDimension<2, 2, 2, 2>>);  // Middle four

    // Test SubDim edge cases with valid ranges
    using SubDim0_6FromLargeRank = typename SubDim<LargeRankTensor, 0, 6>::Type;
    static_assert(std::is_same_v<SubDim0_6FromLargeRank, TDimension<2, 2, 2, 2, 2, 2>>);  // Entire tensor

    using SubDim0_4FromLargeRank = typename SubDim<LargeRankTensor, 0, 4>::Type;
    static_assert(std::is_same_v<SubDim0_4FromLargeRank, TDimension<2, 2, 2, 2>>);  // First four dimensions
  }

  {
    // SubDim Complement Tests - The remaining dimensions after extraction
    using SubDimComplement1_1FromMatrix = typename SubDim<Matrix, 0, 1>::Complement;
    static_assert(std::is_same_v<SubDimComplement1_1FromMatrix, TDimension<4>>);  // Remaining dimension

    using SubDimComplement1_1FromMatrixEnd = typename SubDim<Matrix, 1, 1>::Complement;
    static_assert(std::is_same_v<SubDimComplement1_1FromMatrixEnd, TDimension<3>>);  // First dimension

    using SubDimComplement2_2FromTensor3D = typename SubDim<Tensor3D, 1, 2>::Complement;
    static_assert(std::is_same_v<SubDimComplement2_2FromTensor3D, TDimension<2>>);  // First dimension remains

    using SubDimComplement1_2FromTensor4D = typename SubDim<Tensor4D, 1, 2>::Complement;
    static_assert(std::is_same_v<SubDimComplement1_2FromTensor4D, TDimension<2, 5>>);  // First and last

    using SubDimComplement2_2FromTensor4D = typename SubDim<Tensor4D, 2, 2>::Complement;
    static_assert(std::is_same_v<SubDimComplement2_2FromTensor4D, TDimension<2, 3>>);  // First two

    // Corner cases for SubDim Complement
    using SubDimComplement0_0FromVector = typename SubDim<Vector, 0, 0>::Complement;
    static_assert(std::is_same_v<SubDimComplement0_0FromVector, TDimension<5>>);  // Entire vector remains

    using SubDimComplement0_1FromVector = typename SubDim<Vector, 0, 1>::Complement;
    static_assert(std::is_same_v<SubDimComplement0_1FromVector, TDimension<>>);  // Nothing remains

    using SubDimComplement0_0FromScalar = typename SubDim<Scalar, 0, 0>::Complement;
    static_assert(std::is_same_v<SubDimComplement0_0FromScalar, TDimension<>>);  // Scalar complement

    using SubDimComplement0_2FromMatrix = typename SubDim<Matrix, 0, 2>::Complement;
    static_assert(std::is_same_v<SubDimComplement0_2FromMatrix, TDimension<>>);  // No complement for full matrix

    using SubDimComplement1_1FromSingleRow = typename SubDim<SingleRowMatrix, 1, 1>::Complement;
    static_assert(std::is_same_v<SubDimComplement1_1FromSingleRow, TDimension<1>>);  // First dimension

    using SubDimComplement0_1FromSingleCol = typename SubDim<SingleColMatrix, 0, 1>::Complement;
    static_assert(std::is_same_v<SubDimComplement0_1FromSingleCol, TDimension<1>>);  // Second dimension

    using SubDimComplement1_2FromMixed = typename SubDim<MixedTensor, 1, 2>::Complement;
    static_assert(std::is_same_v<SubDimComplement1_2FromMixed, TDimension<1, 3>>);  // First and last

    using SubDimComplement2_2FromLargeRank = typename SubDim<LargeRankTensor, 2, 2>::Complement;
    static_assert(std::is_same_v<SubDimComplement2_2FromLargeRank, TDimension<2, 2, 2, 2>>);  // First two + last two

    using SubDimComplement1_4FromLargeRank = typename SubDim<LargeRankTensor, 1, 4>::Complement;
    static_assert(std::is_same_v<SubDimComplement1_4FromLargeRank, TDimension<2, 2>>);  // First + last

    using SubDimComplement0_4FromLargeRank = typename SubDim<LargeRankTensor, 0, 4>::Complement;
    static_assert(std::is_same_v<SubDimComplement0_4FromLargeRank, TDimension<2, 2>>);  // Last two dimensions

    // Test that SubDim::Type + SubDim::Complement contains all original dimensions
    // For Matrix<3,4>: SubDim<Matrix, 0, 1> extracts <3>, complement should be <4>
    static_assert(Matrix::count == SubDim<Matrix, 0, 1>::Type::count * SubDimComplement1_1FromMatrix::count);

    // For Tensor3D<2,3,4>: SubDim<Tensor3D, 1, 2> extracts <3,4>, complement should be <2>
    static_assert(Tensor3D::count == SubDim<Tensor3D, 1, 2>::Type::count * SubDimComplement2_2FromTensor3D::count);
  }

  {
    // ContainsDim Tests - Check if one dimension type is contained within another as a contiguous subsequence
    
    // Empty dimension (always contained)
    static_assert(ContainsDim<Scalar, Scalar>::value);           // Scalar contains scalar (empty)
    static_assert(ContainsDim<Vector, Scalar>::value);           // Any dimension contains empty
    static_assert(ContainsDim<Matrix, Scalar>::value);           // Any dimension contains empty
    static_assert(ContainsDim<Tensor3D, Scalar>::value);         // Any dimension contains empty
    
    // Basic containment tests (should work now)
    static_assert(ContainsDim<Matrix, TDimension<3>>::value);     // Matrix<3,4> contains <3> (first dimension)
    static_assert(ContainsDim<Matrix, TDimension<4>>::value);     // Matrix<3,4> contains <4> (second dimension)
    static_assert(ContainsDim<Matrix, TDimension<3,4>>::value);   // Matrix<3,4> contains <3,4> (exact match)
    
    // Self-containment (always true)
    static_assert(ContainsDim<Vector, Vector>::value);           // Vector<5> contains Vector<5>
    static_assert(ContainsDim<Matrix, Matrix>::value);           // Matrix<3,4> contains Matrix<3,4>
    static_assert(ContainsDim<Tensor3D, Tensor3D>::value);       // Tensor3D<2,3,4> contains Tensor3D<2,3,4>
    
    // More basic containment tests
    static_assert(ContainsDim<Tensor3D, TDimension<2>>::value);   // Tensor3D<2,3,4> contains <2> (first dimension)
    static_assert(ContainsDim<Tensor3D, TDimension<3>>::value);   // Tensor3D<2,3,4> contains <3> (second dimension)  
    static_assert(ContainsDim<Tensor3D, TDimension<4>>::value);   // Tensor3D<2,3,4> contains <4> (third dimension)
    static_assert(ContainsDim<Tensor3D, TDimension<2,3>>::value); // Tensor3D<2,3,4> contains <2,3> (first two)
    static_assert(ContainsDim<Tensor3D, TDimension<3,4>>::value); // Tensor3D<2,3,4> contains <3,4> (last two)
    
    // Tensor4D containment tests
    static_assert(ContainsDim<Tensor4D, TDimension<2>>::value);   // Tensor4D<2,3,4,5> contains <2>
    static_assert(ContainsDim<Tensor4D, TDimension<3>>::value);   // Tensor4D<2,3,4,5> contains <3>
    static_assert(ContainsDim<Tensor4D, TDimension<4>>::value);   // Tensor4D<2,3,4,5> contains <4>
    static_assert(ContainsDim<Tensor4D, TDimension<5>>::value);   // Tensor4D<2,3,4,5> contains <5>
    static_assert(ContainsDim<Tensor4D, TDimension<2,3>>::value); // Tensor4D<2,3,4,5> contains <2,3>
    static_assert(ContainsDim<Tensor4D, TDimension<3,4>>::value); // Tensor4D<2,3,4,5> contains <3,4>
    static_assert(ContainsDim<Tensor4D, TDimension<4,5>>::value); // Tensor4D<2,3,4,5> contains <4,5>
    static_assert(ContainsDim<Tensor4D, TDimension<2,3,4>>::value); // Tensor4D<2,3,4,5> contains <2,3,4>
    static_assert(ContainsDim<Tensor4D, TDimension<3,4,5>>::value); // Tensor4D<2,3,4,5> contains <3,4,5>
    static_assert(ContainsDim<Tensor4D, Tensor4D>::value);        // Tensor4D<2,3,4,5> contains itself
    
    // Special cases with single element dimensions
    static_assert(ContainsDim<SingleElementVector, TDimension<1>>::value); // SingleElementVector<1> contains <1>
    static_assert(ContainsDim<SingleRowMatrix, TDimension<1>>::value);      // SingleRowMatrix<1,10> contains <1> (first)
    static_assert(ContainsDim<SingleRowMatrix, TDimension<10>>::value);     // SingleRowMatrix<1,10> contains <10> (second)
    static_assert(ContainsDim<SingleRowMatrix, TDimension<1,10>>::value);   // SingleRowMatrix<1,10> contains <1,10> (exact)
    static_assert(ContainsDim<SingleColMatrix, TDimension<10>>::value);     // SingleColMatrix<10,1> contains <10> (first)
    static_assert(ContainsDim<SingleColMatrix, TDimension<1>>::value);      // SingleColMatrix<10,1> contains <1> (second)
    static_assert(ContainsDim<SingleColMatrix, TDimension<10,1>>::value);   // SingleColMatrix<10,1> contains <10,1> (exact)
    
    // Mixed tensor cases
    static_assert(ContainsDim<MixedTensor, TDimension<1>>::value);          // MixedTensor<1,5,1,3> contains <1> (first or third)
    static_assert(ContainsDim<MixedTensor, TDimension<5>>::value);          // MixedTensor<1,5,1,3> contains <5> (second)
    static_assert(ContainsDim<MixedTensor, TDimension<3>>::value);          // MixedTensor<1,5,1,3> contains <3> (fourth)
    static_assert(ContainsDim<MixedTensor, TDimension<1,5>>::value);        // MixedTensor<1,5,1,3> contains <1,5> (first two)
    static_assert(ContainsDim<MixedTensor, TDimension<5,1>>::value);        // MixedTensor<1,5,1,3> contains <5,1> (second+third)
    static_assert(ContainsDim<MixedTensor, TDimension<1,3>>::value);        // MixedTensor<1,5,1,3> contains <1,3> (third+fourth)
    static_assert(ContainsDim<MixedTensor, MixedTensor>::value);            // Contains itself
    
    // Large rank tensor cases
    static_assert(ContainsDim<LargeRankTensor, TDimension<2>>::value);      // LargeRankTensor<2,2,2,2,2,2> contains <2>
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2>>::value);    // Contains <2,2>
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2,2>>::value);  // Contains <2,2,2>
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2,2,2>>::value); // Contains <2,2,2,2>
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2,2,2,2>>::value); // Contains <2,2,2,2,2>
    static_assert(ContainsDim<LargeRankTensor, LargeRankTensor>::value);    // Contains itself
    
    // Non-containment cases (different values)
    static_assert(!ContainsDim<Vector, Matrix>::value);          // Vector<5> does not contain Matrix<3,4> (rank mismatch)
    static_assert(!ContainsDim<Matrix, Tensor3D>::value);        // Matrix<3,4> does not contain Tensor3D<2,3,4> (rank mismatch)
    static_assert(!ContainsDim<Vector, TDimension<6>>::value);   // Vector<5> does not contain <6> (different value)
    static_assert(!ContainsDim<Matrix, TDimension<2,4>>::value); // Matrix<3,4> does not contain <2,4> (first dimension differs)
    static_assert(!ContainsDim<Matrix, TDimension<3,5>>::value); // Matrix<3,4> does not contain <3,5> (second dimension differs)
    static_assert(!ContainsDim<Tensor3D, TDimension<2,5>>::value); // Tensor3D<2,3,4> does not contain <2,5> (gap in sequence)
    
    // Non-contiguous subsequences (should not be contained)
    static_assert(!ContainsDim<Tensor4D, TDimension<2,4>>::value); // Tensor4D<2,3,4,5> does not contain <2,4> (gap: missing 3)
    static_assert(!ContainsDim<Tensor4D, TDimension<2,5>>::value); // Tensor4D<2,3,4,5> does not contain <2,5> (gap: missing 3,4)
    static_assert(!ContainsDim<Tensor4D, TDimension<3,5>>::value); // Tensor4D<2,3,4,5> does not contain <3,5> (gap: missing 4)
    static_assert(!ContainsDim<Tensor4D, TDimension<2,4,5>>::value); // Tensor4D<2,3,4,5> does not contain <2,4,5> (gap: missing 3)
    
    // More non-containment cases
    static_assert(!ContainsDim<LargeRankTensor, TDimension<3>>::value);     // Does not contain <3>
    static_assert(!ContainsDim<LargeRankTensor, TDimension<2,3>>::value);   // Does not contain <2,3>
    static_assert(!ContainsDim<MixedTensor, TDimension<1,1>>::value);       // MixedTensor<1,5,1,3> does not contain <1,1> (gap: 5 between)
    static_assert(!ContainsDim<MixedTensor, TDimension<5,3>>::value);       // MixedTensor<1,5,1,3> does not contain <5,3> (gap: 1 between)
    static_assert(!ContainsDim<SingleColMatrix, TDimension<1,10>>::value);  // SingleColMatrix<10,1> does not contain <1,10> (wrong order)

    // ContainsDim Index Tests - Test the index member that indicates where D2 occurs in D1
    
    // Empty dimension tests (index should be 0)
    static_assert(ContainsDim<Scalar, Scalar>::index == 0);           // Scalar contains scalar at index 0
    static_assert(ContainsDim<Vector, Scalar>::index == 0);           // Any dimension contains empty at index 0
    static_assert(ContainsDim<Matrix, Scalar>::index == 0);           // Any dimension contains empty at index 0
    static_assert(ContainsDim<Tensor3D, Scalar>::index == 0);         // Any dimension contains empty at index 0

    // Self-containment tests (index should be 0)
    static_assert(ContainsDim<Vector, Vector>::index == 0);           // Vector<5> contains Vector<5> at index 0
    static_assert(ContainsDim<Matrix, Matrix>::index == 0);           // Matrix<3,4> contains Matrix<3,4> at index 0
    static_assert(ContainsDim<Tensor3D, Tensor3D>::index == 0);       // Tensor3D<2,3,4> contains Tensor3D<2,3,4> at index 0

    // Single dimension containment tests
    static_assert(ContainsDim<Matrix, TDimension<3>>::index == 0);     // Matrix<3,4> contains <3> at index 0 (first dimension)
    static_assert(ContainsDim<Matrix, TDimension<4>>::index == 1);     // Matrix<3,4> contains <4> at index 1 (second dimension)
    
    static_assert(ContainsDim<Tensor3D, TDimension<2>>::index == 0);   // Tensor3D<2,3,4> contains <2> at index 0 (first dimension)
    static_assert(ContainsDim<Tensor3D, TDimension<3>>::index == 1);   // Tensor3D<2,3,4> contains <3> at index 1 (second dimension)  
    static_assert(ContainsDim<Tensor3D, TDimension<4>>::index == 2);   // Tensor3D<2,3,4> contains <4> at index 2 (third dimension)

    static_assert(ContainsDim<Tensor4D, TDimension<2>>::index == 0);   // Tensor4D<2,3,4,5> contains <2> at index 0
    static_assert(ContainsDim<Tensor4D, TDimension<3>>::index == 1);   // Tensor4D<2,3,4,5> contains <3> at index 1
    static_assert(ContainsDim<Tensor4D, TDimension<4>>::index == 2);   // Tensor4D<2,3,4,5> contains <4> at index 2
    static_assert(ContainsDim<Tensor4D, TDimension<5>>::index == 3);   // Tensor4D<2,3,4,5> contains <5> at index 3

    // Multi-dimension containment tests
    static_assert(ContainsDim<Matrix, TDimension<3,4>>::index == 0);   // Matrix<3,4> contains <3,4> at index 0 (exact match)
    
    static_assert(ContainsDim<Tensor3D, TDimension<2,3>>::index == 0); // Tensor3D<2,3,4> contains <2,3> at index 0 (first two)
    static_assert(ContainsDim<Tensor3D, TDimension<3,4>>::index == 1); // Tensor3D<2,3,4> contains <3,4> at index 1 (last two)

    static_assert(ContainsDim<Tensor4D, TDimension<2,3>>::index == 0); // Tensor4D<2,3,4,5> contains <2,3> at index 0
    static_assert(ContainsDim<Tensor4D, TDimension<3,4>>::index == 1); // Tensor4D<2,3,4,5> contains <3,4> at index 1
    static_assert(ContainsDim<Tensor4D, TDimension<4,5>>::index == 2); // Tensor4D<2,3,4,5> contains <4,5> at index 2
    static_assert(ContainsDim<Tensor4D, TDimension<2,3,4>>::index == 0); // Tensor4D<2,3,4,5> contains <2,3,4> at index 0
    static_assert(ContainsDim<Tensor4D, TDimension<3,4,5>>::index == 1); // Tensor4D<2,3,4,5> contains <3,4,5> at index 1

    // Special cases with dimensions containing 1s
    static_assert(ContainsDim<SingleElementVector, TDimension<1>>::index == 0); // SingleElementVector<1> contains <1> at index 0
    
    static_assert(ContainsDim<SingleRowMatrix, TDimension<1>>::index == 0);      // SingleRowMatrix<1,10> contains <1> at index 0 (first)
    static_assert(ContainsDim<SingleRowMatrix, TDimension<10>>::index == 1);     // SingleRowMatrix<1,10> contains <10> at index 1 (second)
    static_assert(ContainsDim<SingleRowMatrix, TDimension<1,10>>::index == 0);   // SingleRowMatrix<1,10> contains <1,10> at index 0 (exact)
    
    static_assert(ContainsDim<SingleColMatrix, TDimension<10>>::index == 0);     // SingleColMatrix<10,1> contains <10> at index 0 (first)
    static_assert(ContainsDim<SingleColMatrix, TDimension<1>>::index == 1);      // SingleColMatrix<10,1> contains <1> at index 1 (second)
    static_assert(ContainsDim<SingleColMatrix, TDimension<10,1>>::index == 0);   // SingleColMatrix<10,1> contains <10,1> at index 0 (exact)

    // Mixed tensor cases - test index with repeated values
    static_assert(ContainsDim<MixedTensor, TDimension<1>>::index == 0);          // MixedTensor<1,5,1,3> contains <1> at index 0 (finds first occurrence)
    static_assert(ContainsDim<MixedTensor, TDimension<5>>::index == 1);          // MixedTensor<1,5,1,3> contains <5> at index 1
    static_assert(ContainsDim<MixedTensor, TDimension<3>>::index == 3);          // MixedTensor<1,5,1,3> contains <3> at index 3
    static_assert(ContainsDim<MixedTensor, TDimension<1,5>>::index == 0);        // MixedTensor<1,5,1,3> contains <1,5> at index 0 (first two)
    static_assert(ContainsDim<MixedTensor, TDimension<5,1>>::index == 1);        // MixedTensor<1,5,1,3> contains <5,1> at index 1 (second+third)
    static_assert(ContainsDim<MixedTensor, TDimension<1,3>>::index == 2);        // MixedTensor<1,5,1,3> contains <1,3> at index 2 (third+fourth)

    // Large rank tensor cases - all same values
    static_assert(ContainsDim<LargeRankTensor, TDimension<2>>::index == 0);      // LargeRankTensor<2,2,2,2,2,2> contains <2> at index 0 (first occurrence)
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2>>::index == 0);    // Contains <2,2> at index 0 (first occurrence)
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2,2>>::index == 0);  // Contains <2,2,2> at index 0
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2,2,2>>::index == 0); // Contains <2,2,2,2> at index 0
    static_assert(ContainsDim<LargeRankTensor, TDimension<2,2,2,2,2>>::index == 0); // Contains <2,2,2,2,2> at index 0
    static_assert(ContainsDim<LargeRankTensor, LargeRankTensor>::index == 0);    // Contains itself at index 0

    // Edge case: test that index makes sense for complex patterns
    using ComplexTensor = TDimension<7, 3, 4, 3, 4, 8>;  // Pattern where <3,4> appears at index 1 and 3
    static_assert(ContainsDim<ComplexTensor, TDimension<7>>::index == 0);        // <7> at index 0
    static_assert(ContainsDim<ComplexTensor, TDimension<3>>::index == 1);        // <3> at index 1 (first occurrence)
    static_assert(ContainsDim<ComplexTensor, TDimension<4>>::index == 2);        // <4> at index 2 (first occurrence)
    static_assert(ContainsDim<ComplexTensor, TDimension<8>>::index == 5);        // <8> at index 5
    static_assert(ContainsDim<ComplexTensor, TDimension<3,4>>::index == 1);      // <3,4> at index 1 (first occurrence)
    static_assert(ContainsDim<ComplexTensor, TDimension<4,3>>::index == 2);      // <4,3> at index 2
    static_assert(ContainsDim<ComplexTensor, TDimension<3,4,8>>::index == 3);    // <3,4,8> at index 3
  }

  {
    // CanDegenerate Tests - FromDim contains ToDim as subdimension and rest dimensions are all ones
    // Definition: FromDim can degenerate to ToDim if:
    // 1. FromDim::rank > ToDim::rank (downcast)
    // 2. FromDim contains ToDim as contiguous subsequence
    // 3. The remaining dimensions (complement) are either empty or all ones

    // Basic valid degeneration cases
        // Basic single-one cases: <1,5> -> <5>, <5,1> -> <5>
        // Multiple ones: <1,1,5> -> <5>, <1,5,1> -> <5>, <5,1,1> -> <5>
        // Matrix degenerations: <1,3,4> -> <3,4>, <3,4,1> -> <3,4>
        // Complex patterns: <1,1,1,2,3,4> -> <2,3,4>, <2,3,4,1,1,1> -> <2,3,4>
        // Higher dimensions: <1,1,1,5> -> <5>, <5,1,1,1> -> <5>
        // Scalar degeneration: <1,1,1> -> <> (all ones to scalar)
      
    // Case 1: Higher rank to lower rank with all ones in complement
    static_assert(CanDegenerate<TDimension<1, 5>, TDimension<5>>::value);        // <1,5> -> <5>, complement <1> are all ones
    static_assert(CanDegenerate<TDimension<5, 1>, TDimension<5>>::value);        // <5,1> -> <5>, complement <1> are all ones
    static_assert(CanDegenerate<TDimension<1, 3, 4>, TDimension<3, 4>>::value);  // <1,3,4> -> <3,4>, complement <1> are all ones
    static_assert(CanDegenerate<TDimension<3, 4, 1>, TDimension<3, 4>>::value);  // <3,4,1> -> <3,4>, complement <1> are all ones
    
    // Case 2: Multiple ones in complement
    static_assert(CanDegenerate<TDimension<1, 1, 5>, TDimension<5>>::value);     // <1,1,5> -> <5>, complement <1,1> are all ones
    static_assert(CanDegenerate<TDimension<1, 5, 1>, TDimension<5>>::value);     // <1,5,1> -> <5>, complement <1,1> are all ones
    static_assert(CanDegenerate<TDimension<5, 1, 1>, TDimension<5>>::value);     // <5,1,1> -> <5>, complement <1,1> are all ones
    
    static_assert(CanDegenerate<TDimension<1, 1, 3, 4>, TDimension<3, 4>>::value);  // <1,1,3,4> -> <3,4>, complement <1,1> are all ones
    static_assert(CanDegenerate<TDimension<1, 3, 4, 1>, TDimension<3, 4>>::value);  // <1,3,4,1> -> <3,4>, complement <1,1> are all ones
    static_assert(CanDegenerate<TDimension<3, 4, 1, 1>, TDimension<3, 4>>::value);  // <3,4,1,1> -> <3,4>, complement <1,1> are all ones
    
    // Case 3: Complex patterns with ones
    static_assert(CanDegenerate<TDimension<1, 1, 1, 2, 3, 4>, TDimension<2, 3, 4>>::value);  // Leading ones
    static_assert(CanDegenerate<TDimension<2, 3, 4, 1, 1, 1>, TDimension<2, 3, 4>>::value);  // Trailing ones
    static_assert(CanDegenerate<TDimension<1, 2, 3, 4, 1>, TDimension<2, 3, 4>>::value);     // Mixed ones
    
    // Case 4: Single element vector cases
    static_assert(CanDegenerate<TDimension<1, 1>, TDimension<1>>::value);        // <1,1> -> <1>, one complement dimension
    static_assert(CanDegenerate<TDimension<1, 1, 1>, TDimension<1>>::value);     // <1,1,1> -> <1>, two complement dimensions
    
    // Case 5: Matrix to vector degenerations
    static_assert(CanDegenerate<TDimension<1, 5>, TDimension<5>>::value);        // Row vector to vector
    static_assert(CanDegenerate<TDimension<5, 1>, TDimension<5>>::value);        // Column vector to vector
    
    // Case 6: Tensor to matrix degenerations
    static_assert(CanDegenerate<TDimension<1, 3, 4>, TDimension<3, 4>>::value);  // 3D to 2D
    static_assert(CanDegenerate<TDimension<3, 4, 1>, TDimension<3, 4>>::value);  // 3D to 2D (last dimension is 1)
    
    // Case 7: Higher dimension degenerations
    static_assert(CanDegenerate<TDimension<1, 1, 1, 5>, TDimension<5>>::value);  // 4D to 1D (all leading ones)
    static_assert(CanDegenerate<TDimension<5, 1, 1, 1>, TDimension<5>>::value);  // 4D to 1D (all trailing ones)
    
    // Invalid degeneration cases (should be false)
    
    // Case 1: Same rank (no downcast)
    static_assert(!CanDegenerate<TDimension<5>, TDimension<5>>::value);          // Same rank, no degeneration
    static_assert(!CanDegenerate<TDimension<3, 4>, TDimension<3, 4>>::value);    // Same rank, no degeneration
    
    // Case 2: Non-one values in complement
    static_assert(!CanDegenerate<TDimension<2, 5>, TDimension<5>>::value);       // <2,5> -> <5>, complement <2> is not 1
    static_assert(!CanDegenerate<TDimension<5, 2>, TDimension<5>>::value);       // <5,2> -> <5>, complement <2> is not 1
    static_assert(!CanDegenerate<TDimension<2, 3, 4>, TDimension<3, 4>>::value); // <2,3,4> -> <3,4>, complement <2> is not 1
    static_assert(!CanDegenerate<TDimension<3, 4, 2>, TDimension<3, 4>>::value); // <3,4,2> -> <3,4>, complement <2> is not 1
    
    // Case 3: Mixed ones and non-ones in complement
    static_assert(!CanDegenerate<TDimension<1, 2, 5>, TDimension<5>>::value);    // <1,2,5> -> <5>, complement <1,2> has non-one
    static_assert(!CanDegenerate<TDimension<2, 1, 5>, TDimension<5>>::value);    // <2,1,5> -> <5>, complement <2,1> has non-one
    static_assert(!CanDegenerate<TDimension<5, 1, 2>, TDimension<5>>::value);    // <5,1,2> -> <5>, complement <1,2> has non-one
    
    // Edge cases with complex patterns
        // Same rank: <5> -> <5>, <3,4> -> <3,4> (no downcast)
        // Non-one complements: <2,5> -> <5>, <5,2> -> <5> (complement has 2, not 1)
        // Mixed complements: <1,2,5> -> <5>, <2,1,5> -> <5> (complement has both 1s and non-1s)
        // Complex invalid cases: Patterns where the complement contains values other than 1 

    // Case 1: Multiple possible matches - this case doesn't work because complement has non-ones
    using MultiPattern = TDimension<1, 3, 4, 1, 3, 4, 1>;  // <3,4> appears at index 1 and 4
    static_assert(!CanDegenerate<MultiPattern, TDimension<3, 4>>::value);       // Complement has non-ones
    
    // Case 2: Complex tensor degenerations
    static_assert(CanDegenerate<TDimension<1, 1, 2, 3, 1, 1>, TDimension<2, 3>>::value); // Multiple ones around target
    static_assert(!CanDegenerate<TDimension<2, 2, 3, 2, 2>, TDimension<2, 3>>::value);   // Non-ones in complement
    
    // Case 3: Large rank differences
    static_assert(CanDegenerate<TDimension<1, 1, 1, 1, 5, 1, 1>, TDimension<5>>::value); // 7D to 1D with all ones
    static_assert(!CanDegenerate<TDimension<2, 1, 1, 1, 5, 1, 1>, TDimension<5>>::value); // One non-one value
    
    // Case 4: Scalar degenerations - this actually works! All ones can degenerate to scalar
    static_assert(CanDegenerate<TDimension<1, 1, 1>, TDimension<>>::value);     // All ones can degenerate to scalar
  }

}


TEST(Math, Dimension)
{
  test();
}
