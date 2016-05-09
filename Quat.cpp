#include "Quat.h"
Quat::Quat(){
	axis = Vec3(0.0f, 0.0f, 0.0f);
	angle = 1.0f;
}

Quat::Quat(const Vec3& newAxis, float newAngle){
	axis = newAxis;
	angle = newAngle;
}

/**
 * Overloaded constructor initializes the quaternion from a real scalar and imaginary vector
 */
Quat::Quat(float scalar, const Vec3& vector)
    : axis(vector[0] / sqrt(1 - scalar * scalar), vector[1] / sqrt(1 - scalar * scalar),
	    vector[2] / sqrt(1 - scalar * scalar))
    , angle(2 * acos(scalar))
{
    /* empty */
} /* Overloaded constructor */

/**
 * Rotates a 3D vector by the quaternion's axis and angle
 * @param rhs - The vector to rotate
 * @return - The rotated vector
 */
Vec3 Quat::operator*(const Vec3& rhs)
{
	Vec4 v;
	float delta = sin(angle / 2);
	float gamma = cos(angle / 2);
	Mat4 Q_bar = { gamma	,  delta * axis[2], -delta * axis[1], delta * axis[0]
				,-delta * axis[2],  gamma         ,  delta * axis[0], delta * axis[1]
				, delta * axis[1], -delta * axis[0],  gamma         , delta * axis[2]
				,-delta * axis[0], -delta * axis[1], -delta * axis[2], gamma         };
	Mat4 Q = { gamma        ,delta * axis[2], -delta * axis[1], -delta * axis[0]
				,-delta * axis[2],  gamma         ,  delta * axis[0], -delta * axis[1]
				, delta * axis[1], -delta * axis[0],  gamma         , -delta * axis[2]
				, delta * axis[0],  delta * axis[1],  delta * axis[2],  gamma         };
	v = Vec4(rhs[0], rhs[1], rhs[2], 0.0f);
	    
		Mat4 temp = Q_bar * Q;
		v = temp * v;

	return Vec3(v[0], v[1], v[2]);
} 
