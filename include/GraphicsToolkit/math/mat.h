#ifndef MAT_H
#define MAT_H

#include "impl/mat_op.h"
#include "impl/mat_temp.h"


using Mat2f = Mat<float, 2, 2>;
using Mat3f = Mat<float, 3, 3>;
using Mat4f = Mat<float, 4, 4>;

using Mat2d = Mat<double, 2, 2>;
using Mat3d = Mat<double, 3, 3>;
using Mat4d = Mat<double, 4, 4>;

#endif  // MAT_H