#ifndef SHAPE_TRAITS_H
#define SHAPE_TRAITS_H


template<typename Scalar, size_t m, size_t n>
class Mat;

template<typename Scalar, size_t l>
class Vec;

template<typename T>
struct ShapeTraits {};

template<typename Scalar, size_t len>
struct ShapeTraits<Mat<Scalar, len, 1>> {
  using AsColVec = Vec<Scalar, len>;
  using AsColMat = Mat<Scalar, len, 1>;
};

template<typename Scalar, size_t len>
struct ShapeTraits<Mat<Scalar, 1, len>> {
  using AsRowVec = Vec<Scalar, len>;
  using AsRowMat = Mat<Scalar, 1, len>;
};

template<typename Scalar, size_t len>
struct ShapeTraits<Vec<Scalar, len>> {
  using AsRowVec = Vec<Scalar, len>;
  using AsColVec = Vec<Scalar, len>;
  using AsRowMat = Mat<Scalar, 1, len>;
  using AsColMat = Mat<Scalar, len, 1>;
};


#endif  // SHAPE_TRAITS_HH