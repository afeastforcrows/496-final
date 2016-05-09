/*
 * Copyright (c) 2005-2013 Michael Shafae
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * $Id: Primitive.h 4472 2013-09-29 07:43:28Z mshafae $
 *
 */

/*
 * This is not a high quality PLY parser. It works well enough.
 */


#include "PlyModel.h"
#include "GFXMath.h"
//#include "GFXExtra.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>


#ifndef SQR
#define SQR( x ) ((x) * (x))
#endif



int vecPrint3d(FILE* f, double *v){
  return fprintf( f, "(%.16f, %.16f, %.16f)\n", v[0], v[1], v[2] );
}

void vecCopy3d(double *dest, double *src){
  int i;
  for(i = 0; i < 3; i++){
    dest[i] = src[i];
  }
}

double vecSquaredLength3d(double *a){
  double accumulate = 0.0;
  int i;
  for(i = 0; i < 3; i++){
    accumulate += SQR( a[i] );
  }
  return( accumulate );
}


double vecLength3d(double *a){
  return(sqrt( vecSquaredLength3d(a) ) );
}

void vecDifference3d(double *c, double *a, double *b){
  int i;
  for(i = 0; i < 3; i++){
    c[i] = a[i] - b[i];
  }
}


void vecCross3d(double *n, double *u, double *v){
  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0]; 
}


void vecNormalize3d(double *vout, double* v){
  double length;
  length = vecLength3d(v);
   if( length <= 0.0 ){
     vout[0] = 0.0;
     vout[1] = 0.0;
     vout[2] = 0.0;
   }
   if( length == 1.0 ){
     vecCopy3d(vout, v);
   }else{
    length = 1.0 / length;
    vout[0] = v[0] * length;
    vout[1] = v[1] * length;
    vout[2] = v[2] * length;
  }
}

double vecDot3d(double *a, double *b){
  double accumulate = 0.0;
  int i;
  for(i = 0; i < 3; i++){
    accumulate += a[i] * b[i];
  }
  return( accumulate );
}

void vecCalcNormal3d(double *n, double *p, double *p1, double *p2){
  double pa[3],pb[3];

  vecDifference3d(pa, p1, p);
  vecDifference3d(pb, p2, p);

  vecCross3d(n, pa, pb);
  vecNormalize3d(n, n);
}

double vecDistanceBetween3d(double *a, double *b ){
  double c[3];
  vecDifference3d(c, a, b);
  return( vecLength3d(c) ); 
}

double vecSquaredDistanceBetween3d(double *a, double *b ){
  double c[3];
  vecDifference3d(c, a, b);
  return( vecSquaredLength3d(c) ); 
}


void vecSum3d(double *c, double *a, double *b){
  int i;
  for(i = 0; i < 3; i++){
    c[i] = a[i] + b[i];
  }
}

double r(){
  return double(rand( ))/double(RAND_MAX);
}

void midpoint(double *m, double *a, double *b){
  double s[3];
  vecSum3d(s, a, b);
  for(int i = 0; i < 3; i++ ){
    m[i] = s[i] * 0.5;
  }
}

void calcRitterBoundingSphere(double* center, double* radius, FaceList *fl){
  /*
  An Efficient Bounding Sphere
  by Jack Ritter
  from "Graphics Gems", Academic Press, 1990
  p. 301-303, code: p. 723-725
  */
  
  int count = 0;
  
  double x_min, x_max, y_min, y_max, z_min, z_max;
  int px_min, px_max, py_min, py_max, pz_min, pz_max;

  double *p;
  
  // Intialize
  // Select a point in the mesh as an arbitrary starting point.
  p = fl->vertices[0];
  px_min = px_max = py_min = py_max = pz_min = pz_max = 0;
  x_min = x_max = p[0];
  y_min = y_max = p[1];
  z_min = z_max = p[2];
  
  // Find 6 minima & maxima points
  for( int i = 0; i < fl->vc; i++ ){
    p = fl->vertices[i];
    if( p[0] < x_min ){
      x_min = p[0];
      px_min = i;
    }
    if( p[0] > x_max ){
      x_max = p[0];
      px_max = i;
    }
    if( p[1] < y_min ){
      y_min = p[1];
      py_min = i;
    }
    if( p[1] > y_max ){
      y_max = p[1];
      py_max = i;
    }
    if( p[2] < z_min ){
      z_min = p[2];
      pz_min = i;
    }
    if( p[2] > z_max ){
      z_max = p[2];
      pz_max = i;
    }
  } // end for

  // Find the distance between the extremal values
  double x_span_dist = vecSquaredDistanceBetween3d(fl->vertices[px_max], fl->vertices[px_min]);
  
  double y_span_dist = vecSquaredDistanceBetween3d(fl->vertices[py_max], fl->vertices[py_min]);
  
  double z_span_dist = vecSquaredDistanceBetween3d(fl->vertices[pz_max], fl->vertices[pz_min]);
  
  // Find the maximally separated pair; dia1 & dia2
  double max_span_dist = x_span_dist;
  int dia1, dia2;
  dia1 = px_min;
  dia2 = px_max;
  if( y_span_dist > max_span_dist ){
    max_span_dist = y_span_dist;
    dia1 = py_min;
    dia2 = py_max;
  }
  if( z_span_dist > max_span_dist ){
    dia1 = pz_min;
    dia2 = pz_max;
  }
  
  // dia1 and dia2 define the diameter of the intial sphere
  // calculate the center of the sphere
  double _center[3];
  midpoint(_center, fl->vertices[dia1], fl->vertices[dia2]);

  // Calculate the length of the radius
  double radius_sq_dist = vecSquaredDistanceBetween3d(fl->vertices[dia2], _center);
  double radius_dist = sqrt(radius_sq_dist);
  
  // Incrementally grow the sphere to include all points
  for( int i = 0; i < fl->vc; i++ ){
    double test_sq_dist = vecSquaredDistanceBetween3d(fl->vertices[i], _center);
    if( test_sq_dist > radius_sq_dist ){
      // the current (ith) point is outside the sphere
      double test_dist = sqrt(test_sq_dist);
      // calculate the radius of the new sphere
      radius_dist = (radius_dist + test_dist) * 0.5;
      radius_sq_dist = SQR(radius_dist);
      // calculate the center of the new sphere
      double old_to_new_dist = test_dist - radius_dist;
      for(int j = 0; j < 3; j++){
        _center[j] = (radius_dist * _center[j] + old_to_new_dist * fl->vertices[i][j]) / test_dist;
      }
      count++;
    }
  } // end for
  *radius = radius_dist;
  for(int j = 0; j < 3; j++){
    center[j] = _center[j];
  }
  //printf("Grew the sphere %d times\n", count);
}

void calcBoundingSphere(double* center, double* radius, FaceList *fl){
  double maxDistance = 0.0;
  for( int i = 0; i < fl->vc-1; i++ ){
    for(int j = i + 1; j < fl->vc; j++){
      double *a = fl->vertices[i];
      double *b = fl->vertices[j];
      double distance = vecSquaredDistanceBetween3d(a, b);
      if( distance > maxDistance){
        midpoint(center, a, b);
        *radius = sqrt(distance) * 0.5;
        maxDistance = distance;
      }
    }
  }
}

FaceList* readPlyModel( const char* filename ){
  char buffer[255], type[128], c;
  std::ifstream inputfile;
  unsigned int i;
  int k;
  unsigned int nv;
  unsigned int nf;
  FaceList *fl;
  assert( filename );
  inputfile.open( filename, std::ios::in );
  if( inputfile.fail( ) ){
    std::cerr << "File \"" << filename << "\" not found." << std::endl;
    exit( 1 );
  }
  // Parse the header
  if(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
    if( strcmp(buffer, "ply") != 0){
      std::cerr << "Error: Input file is not of .ply type." << std::endl;
      exit(1);
    }
  }else{
    std::cerr << "End of input?" << std::endl;
    exit( 1 );
  }
  if(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
    if( strncmp(buffer, "format ascii", 12) != 0){
      std::cerr << "Error: Input file is not in ASCII format." << std::endl;
      exit(1);
    }
  }else{
    std::cerr << "End of input?" << std::endl;
    exit( 1 );
  }
  if(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
    while (strncmp(buffer, "comment", 7) == 0){
      inputfile.getline(buffer, sizeof(buffer), '\n');
  }
  }else{
    std::cerr << "End of input?" << std::endl;
    exit( 1 );
  }

  if (strncmp(buffer, "element vertex", 14) == 0){
    sscanf(buffer, "element vertex %u\n", &nv);
  }else{
    std::cerr << "Error: number of vertices expected." << std::endl;
    exit(1);
  }

  i = 0;
  inputfile.getline(buffer, sizeof(buffer), '\n');
  while (strncmp(buffer, "property", 8) == 0) {
    if (i < 3) {
      sscanf(buffer, "property %s %c\n", type, &c);
      switch (i) {
      case 0:
        if (c != 'x') {
          std::cerr << "Error: first coordinate is not x." << std::endl;
          exit(1);
        }
        break;
      case 1:
        if (c != 'y') {
          std::cerr << "Error: first coordinate is not y." << std::endl;
          exit(1);
        }
        break;
      case 2:
        if (c != 'z') {
          std::cerr << "Error: first coordinate is not z." << std::endl;
          exit(1);
        }
        break;
      default:
        break;
      }
      i++;
    }
    inputfile.getline(buffer, sizeof(buffer), '\n');
  }
  
  if (strncmp(buffer, "element face", 12) == 0)
    sscanf(buffer, "element face %u\n", &nf);
  else {
    std::cerr << "Error: number of faces expected." << std::endl;
    exit(1);
  }

  inputfile.getline(buffer, sizeof(buffer), '\n');
  if (strncmp(buffer, "property list", 13) != 0) {
    std::cerr << "Error: property list expected." << std::endl;
    exit(1);
  }
  
  inputfile.getline(buffer, sizeof(buffer), '\n');
  while (strncmp(buffer, "end_header", 10) != 0){
    inputfile.getline(buffer, sizeof(buffer), '\n');
  }
  
  // Allocate FaceList object
  if( !(fl = new FaceList( nv, nf)) ){
    std::cerr << "Could not allocate a new face list for the model." << std::endl;
    exit(1);
  }

  /* Process the body of the input file*/

  // read vertex data from PLY file
  for (i = 0; i < nv; i++) {
    inputfile.getline(buffer, sizeof(buffer), '\n');
    sscanf(buffer,"%lf %lf %lf", &(fl->vertices[i][0]), &(fl->vertices[i][1]), &(fl->vertices[i][2]));
  }

  // read face data from PLY file
  for (i = 0; i < nf; i++) {
    inputfile.getline(buffer, sizeof(buffer), '\n');
    sscanf(buffer, "%d %d %d %d", &k, &(fl->faces[i][0]), &(fl->faces[i][1]), &(fl->faces[i][2]) );
    if (k != 3) {
      fprintf(stderr, "Error: not a triangular face.\n");
      exit(1);
    }
  }

  inputfile.close( );
  
  calcRitterBoundingSphere(fl->center, &(fl->radius), fl);

  for( i = 0; i < nv; i++){
    vecDifference3d(fl->vertices[i], fl->vertices[i], fl->center);
  }

  // compute face normals
  for( i = 0; i < nf; i++){
    vecCalcNormal3d(fl->f_normals[i],
      fl->vertices[fl->faces[i][0]],
      fl->vertices[fl->faces[i][1]],
      fl->vertices[fl->faces[i][2]]);

    for(int j = 0; j < 3; j++){
      vecSum3d(fl->v_normals[fl->faces[i][j]],
        fl->v_normals[fl->faces[i][j]], fl->f_normals[i] );
    }
  }

  // normalize the v_normals
  for( i = 0; i < nv; i++ ){
    for(int j = 0; j < 3; j++){
      // set some colors
      fl->colors[i][j] = r( );
      // Scale the vertices
      fl->vertices[i][j] /= fl->radius;
    }
    vecNormalize3d(fl->v_normals[i], fl->v_normals[i]);
  }

  calcRitterBoundingSphere(fl->center, &(fl->radius), fl);

  return( fl );
}

