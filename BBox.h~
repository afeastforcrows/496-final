#include "GFXMath.h"
#include "PlyModel.h"
#include <cmath>

#ifndef Included_BBox_H
#define Included_BBox_H

class BBox{
	public:
	Vec3 center;
	float width;
	Vec4 vertices[8];
	
	void align();

	Vec3 frontNorm();

	Vec3 topNorm();

	Vec4 translate4(Vec4 myVec, float x, float y, float z, float w);

	Vec4 scale4(Vec4 myVec, float s);

	void update(Vec3 c, float w);

	void drawBB();
};
#endif
