#pragma once

#include <Tensor.h>

template<typename Scalar, size_t row, size_t col>
using Matrix = Tensor<Scalar, row, col>;

