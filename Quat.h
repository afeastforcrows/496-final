#ifndef QUAT_H_
#define QUAT_H_

#define _USE_MATH_DEFINES
#include <cmath>

#include "GFXMath.h"

class Quat{
public:
	Vec3 axis;
	float angle;

	Quat();

	Quat(const Vec3& newAxis, float newAngle);
	Quat(float scalar, const Vec3& vector);
	Vec3 operator*(const Vec3& rhs);

};

#endif
