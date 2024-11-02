#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "mat.h"
#include "vec.h"

class Transform {
public:
	Transform() = default;
	Transform(const Mat4d& matrix);
	Transform(const Vec3d& loc, const Vec3d& rot, double scale);

private:
	Mat4d matrix_ = Mat4d::Identity();
	Mat4d invMatrix_ = Mat4d::Identity();
};



#endif  // TRANSFORM_H