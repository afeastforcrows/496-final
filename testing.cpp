// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// Procedural module that implements transformations used in
// the homework assignment.
//
// $Id: transformations.cpp 5554 2015-02-19 06:59:50Z mshafae $
//
// STUDENTS _MUST_ ADD THEIR CODE INTO THIS FILE
//

#include "transformations.h"

void rotateCameraLeft(float degrees, Vec3& eyePosition, Vec3& centerPosition, Vec3& upVector){
  Vec3 f = normalize(-eyePosition);
  Vec3 _up = normalize(upVector);
  Vec3 s = normalize(cross(f, _up));
  Vec3 u = cross(s, f);
  Mat3 m = rotate3(degrees, u);
  upVector = m * u;
  eyePosition = m * eyePosition;
}

void rotateCameraUp(float degrees, Vec3& eyePosition, Vec3& centerPosition, Vec3& upVector){
  Vec3 f = normalize(-eyePosition);
  Vec3 _up = normalize(upVector);
  Vec3 s = normalize(cross(f, _up));
  Vec3 u = cross(s, f);
  Mat3 m = rotate3(-degrees, s);
  upVector = m * u;
  eyePosition = m * eyePosition;
}
