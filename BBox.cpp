//////////////////////////////////////////////////////////////////
//	Walter Wyatt Dorn					//
//	CPSC 486						//
//	Camera Control & View Frustum Culling – Assignment 3	//
//	Dr. Shafae - CSU Fullerton				//	
//	5/18/16							//
//////////////////////////////////////////////////////////////////

#include "BBox.h"
#include "GFXMath.h"
#include "GFXExtra.h"

void BBox::align(){
	//
}

Vec3 BBox::frontNorm(){
	Vec3 edge1, edge2;
	edge1 = Vec3(vertices[4][0],vertices[4][1],vertices[4][2])-Vec3(vertices[7][0],vertices[7][1],vertices[7][2]);
	edge2 = Vec3(vertices[6][0],vertices[6][1],vertices[6][2])-Vec3(vertices[7][0],vertices[7][1],vertices[7][2]);
	//Vec3 temp = normalize(cross(edge1,edge2));
	//fprintf( stderr, "Cross: %f, %f, %f\n", temp[0], temp[1], temp[2]);
	return normalize(cross(edge1,edge2));
}

Vec3 BBox::topNorm(){
	Vec3 edge1, edge2;
	edge1 = Vec3(vertices[2][0],vertices[2][1],vertices[2][2])-Vec3(vertices[7][0],vertices[7][1],vertices[7][2]);
	edge2 = Vec3(vertices[3][0],vertices[3][1],vertices[3][2])-Vec3(vertices[7][0],vertices[7][1],vertices[7][2]);
	//Vec3 temp = cross(edge1,edge2);
	//fprintf( stderr, "Cross: %f, %f, %f\n", temp[0], temp[1], temp[2]);
	return normalize(cross(edge1,edge2));
}

Vec4 BBox::translate4(Vec4 myVec, float x, float y, float z, float w){
		myVec[0]+=x;
		myVec[1]+=y;
		myVec[2]+=z;
		myVec[3]+=w;
	return myVec;
}

Vec4 BBox::scale4(Vec4 myVec, float s){
		myVec[0] *= s;
		myVec[1] *= s;
		myVec[2] *= s;
		myVec[3] *= s;
	return myVec;
}

void BBox::update(Vec3 c, float w){
	center = c;
	width = w;
}

void BBox::drawBB(){//Vec3 center, float width){

	//gluSphere(quadObj, radius, 8, 8);

	vertices[0] = Vec4(	-0.5,	-0.5,	-0.5,	1.0);
	vertices[1] = Vec4(	0.5,	-0.5,	-0.5,	1.0);
	vertices[2] = Vec4(	0.5,	0.5,	-0.5,	1.0);
	vertices[3] = Vec4(	-0.5,	0.5,	-0.5,	1.0);
	vertices[4] = Vec4(	-0.5,	-0.5,	0.5,	1.0);
	vertices[5] = Vec4(	0.5,	-0.5,	0.5,	1.0);
	vertices[6] = Vec4(	0.5,	0.5,	0.5,	1.0);
	vertices[7] = Vec4(	-0.5,	0.5,	0.5,	1.0);
	glLineWidth(2.5);
	for(int i = 0; i<8; i++){
		vertices[i] = scale4(vertices[i], width*2);
	}
	for(int i = 0; i<8; i++){
		vertices[i] = translate4(vertices[i], center[0], center[1], center[2], 1);
	}
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_QUADS);      // draw a cube with 6 quads
	glVertex3fv(vertices[0]);	//back
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);

	glVertex3fv(vertices[0]);	//left
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[4]);

	glVertex3fv(vertices[1]);	//right
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[2]);

	glVertex3fv(vertices[2]);	//top
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[6]);

	glVertex3fv(vertices[1]);	//bottom
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[5]);

	glVertex3fv(vertices[4]);	//front
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[5]);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
