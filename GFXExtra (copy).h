/*
 * Copyright (c) 2015 Michael Shafae
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
 * $Id: GFXExtra.h 5941 2015-09-30 08:11:06Z mshafae $
 *
 */

#ifdef __clang__
#pragma clang diagnostic ignored "-Wunused-function"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#ifndef _GFXEXTRA_H_
#define _GFXEXTRA_H_

#ifdef WIN32
#pragma once
#define _USE_MATH_DEFINES 1
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>

#include <FreeImagePlus.h>

#include "GFXMath.h"

#if (defined(BYTE_ORDER) && BYTE_ORDER==BIG_ENDIAN) || \
        (defined(__BYTE_ORDER) && __BYTE_ORDER==__BIG_ENDIAN) || \
        defined(__BIG_ENDIAN__)
#define GFX_BIG_ENDIAN_BYTE_ORDER
#else
#define GFX_LITTLE_ENDIAN_BYTE_ORDER
#endif


template <typename T, const int length>
class PointN{
public:
  typedef class PointN<T,length> this_t;
  typedef T component_t;

  PointN( ){};

  PointN(const PointN& rhs){
    assign(rhs);
  }
  
  PointN(const PointN* rhs){
    assign(rhs);
  }
  
  PointN(const VecN<T, length> rhs){
    assign(rhs);
  }
  
  PointN(const T* array){
    assign(array);
  }
  
  PointN(T const & s){
    for( int i = 0; i < length; i++ ){
      data[i] = s;
    }
  }
  
  PointN& operator =(const PointN& rhs){
    assign(rhs);
    return *this;
  }

  PointN& operator =(const PointN* rhs){
    assign(rhs);
    return *this;
  }
  
  PointN& operator =(const T& s){
    assign(s);
    return *this;
  }
  
  PointN operator +(const VecN<T, length>& rhs) const{
    PointN displaced;
    for( int i = 0; i < length; i++ ){
      displaced.data[i] = data[i] + rhs[i];
    }
    return displaced;
  }
  
  PointN operator -( ) const{
    PointN rv;
    for( int i = 0; i < length; i++ ){
      rv.data[i] = -data[i];
    }
    return rv;
  }
  
  VecN<T, length> operator -(const PointN& rhs) const{
    T __data[length];
    for( int i = 0; i < length; i++ ){
      __data[i] = data[i] - rhs.data[i];
    }
    return VecN<T, length>(__data);
  }
  
  PointN operator -(const VecN<T, length>& rhs) const{
    PointN displaced;
    for( int i = 0; i < length; i++ ){
      displaced.data[i] = data[i] - rhs[i];
    }
    return displaced;
  }
  
  PointN operator *(const T& s) const{
    PointN rv;
    for(int i = 0; i < length; i++){
      rv.data[i] = data[i] * s;
    }
    return rv;
  }
  
  PointN operator *=(const T& s){
    assign(*this * s);
    return *this;
  }
  
  PointN operator /(const T& s) const{
    PointN rv;
    for( int i = 0; i < length; i++ ){
      rv.data[i] = data[i] / s;
    }
    return rv;
  }
  
  PointN operator /=(const T& s){
    assign(*this / s);
    return *this;
  }
  
  T& operator [](unsigned int i){
    if( i < length ){
      return data[i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  const T& operator [](unsigned int i) const{
    if( i < length ){
      return data[i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  bool operator ==(const PointN& rhs) const{
    bool rv = false;
    float precision = FP_SP_EPSILON;
    for( int i = 0; i < length; i++ ){
      rv = fpEqual(data[i], rhs.data[i], precision);
      if(!rv)
        break;
    }
    return rv;
  }
  
  bool operator !=(const PointN& rhs) const{
    bool rv = false;
    float precision = FP_SP_EPSILON;
    for( int i = 0; i < length; i++ ){
      rv = fpNotEqual(data[i], rhs.data[i], precision);
      if(!rv)
        break;
    }
    return rv;
  }
  
  static int size( ){
    return length;
  }
  
  operator T*( ){
    return &data[0];
  }

  operator const T*( ) const{
    return &data[0];
  }
  
  VecN<T, length> vec( ) const{
    VecN<T, length> v;
    for(int i = 0; i < length; i++){
      v[i] = data[i];
    }
    return v;
  }
  std::ostream& write( std::ostream &out ) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION)
      << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << " " << data[i] << std::endl;
      }else{
        out << " " << data[i] << " )" << std::endl;
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  
  std::ostream& write_row( std::ostream &out) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION)
      << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << data[i] << ", ";
      }else{
        out << data[i] << ")";
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  
  std::ostream& description( std::ostream &out = std::cerr ) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION) <<
      "<Point"<< length << " " << this << "> ";
    out << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << data[i] << ", ";
      }else{
        out << data[i] << ")" << std::endl;
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  
  
protected:
  T data[length];

  inline void assign(const PointN& rhs){
    for( int i = 0; i < length; i++ ){
      data[i] = rhs.data[i];
    }
  }

  inline void assign(const VecN<T, length>& rhs){
    for( int i = 0; i < length; i++ ){
      data[i] = rhs[i];
    }
  }

  inline void assign(const T& s){
    for( int i = 0; i < length; i++ ){
      data[i] = s;
    }
  }
  
  inline void assign(const T* array){
    for( int i = 0; i < length; i++ ){
      data[i] = array[i];
    }
  }
  
  
  inline void assign(const PointN* rhs){
    for( int i = 0; i < length; i++ ){
      data[i] = rhs->data[i];
    }
  }

}; // end class PointN

template <typename T>
class TPoint1 : public PointN<T, 1>{
public:
  typedef PointN<T, 1> base;
  TPoint1( ) : base( ){ }
  TPoint1(const base& v) : base(v){ }
  explicit TPoint1(const base* v) : base(v){ }
  explicit TPoint1(T x){
    base::data[0] = x;
  }
}; // end class TPoint1

template <typename T>
class TPoint2 : public PointN<T, 2>{
public:
  typedef PointN<T, 2> base;
  TPoint2( ) : base( ){ }
  TPoint2(const base& v) : base(v){ }
  explicit TPoint2(const base* v) : base(v){ }
  explicit TPoint2(T x, T y){
    base::data[0] = x;
    base::data[1] = y;
  }
}; // end class TPoint2

template <typename T>
class TPoint3 : public PointN<T, 3>{
public:
  typedef PointN<T, 3> base;
  TPoint3( ) : base( ){ }
  TPoint3(const base& v) : base(v){ }
  explicit TPoint3(const base* v) : base(v){ }
  explicit TPoint3(T x, T y, T z){
    base::data[0] = x;
    base::data[1] = y;
    base::data[2] = z;
  }
  
  explicit TPoint3(const TPoint2<T>& v, T z){
    base::data[0] = v[0];
    base::data[1] = v[1];
    base::data[2] = z;
  }

  explicit TPoint3(const TVec4<T>& v){
    assert(v[3] != T(0));
    base::data[0] = v[0] / v[3];
    base::data[1] = v[1] / v[3];
    base::data[2] = v[2] / v[3];
  }
  
  PointN<T, 2> xy( ){
    return PointN<T, 2>(base::data[0] / base::data[2], base::data[1] / base::data[2]);
  }
  
}; // end class TPoint3

template <typename T>
class TPoint4 : public PointN<T, 4>{
public:
  typedef PointN<T, 4> base;
  TPoint4( ) : base( ){ }
  TPoint4(const base& v) : base(v){ }
  explicit TPoint4(const base* v) : base(v){ }
  explicit TPoint4(T x, T y, T z, T w){
    base::data[0] = x;
    base::data[1] = y;
    base::data[2] = z;
    base::data[3] = w;
  }
  
  explicit TPoint4(const TPoint2<T>& v, T z, T w){
    base::data[0] = v[0];
    base::data[1] = v[1];
    base::data[2] = z;
    base::data[3] = w;
  }
  
  explicit TPoint4(const TPoint3<T>& v, T w){
    base::data[0] = v[0];
    base::data[1] = v[1];
    base::data[2] = v[2];
    base::data[3] = w;
  }

  PointN<T, 3> xyz( ){
    return PointN<T, 3>(base::data[0] / base::data[3], base::data[1] / base::data[3], base::data[2] / base::data[3]);
  }

}; // end class TPoint4

template <typename T, int length>
static const PointN<T, length> operator *(T lhs, const PointN<T, length>& rhs){
  return rhs * lhs;
}

template <typename T>
static const TPoint1<T> operator /(T lhs, const TPoint1<T>& rhs){
  return TPoint1<T>(lhs / rhs[0]);
}

template <typename T>
static const TPoint2<T> operator /(T lhs, const TPoint2<T>& rhs){
  return TPoint2<T>(lhs / rhs[0], lhs / rhs[1]);
}

template <typename T>
static const TPoint3<T> operator /(T lhs, const TPoint3<T>& rhs){
  return TPoint3<T>(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2]);
}

template <typename T>
static const TPoint4<T> operator /(T lhs, const TPoint4<T>& rhs){
  return TPoint4<T>(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2], lhs / rhs[3]);
}

template <typename T, int length>
std::ostream& operator <<( std::ostream &out, const PointN<T, length> &v ){
  return(v.write_row( out ));
}

typedef TPoint1<float> Point1;
typedef TPoint1<double> Point1d;
typedef TPoint1<int> Point1i;

typedef TPoint2<float> Point2;
typedef TPoint2<double> Point2d;
typedef TPoint2<int> Point2i;

typedef TPoint3<float> Point3;
typedef TPoint3<double> Point3d;
typedef TPoint3<int> Point3i;

typedef TPoint4<float> Point4;
typedef TPoint4<double> Point4d;
typedef TPoint4<int> Point4i;

template <typename T>
class TInterval{
public:
  TInterval(const T start, const T stop, const bool isOpen = false):
  _start(start), _stop(stop), _openStart(isOpen), _openStop(isOpen){ };

  TInterval(const T start, const T stop, const bool isOpenStart, const bool isOpenStop):
  _start(start), _stop(stop), _openStart(isOpenStop), _openStop(isOpenStop){ };

  TInterval<T>& operator =(const TInterval<T>& rhs){
    _start = rhs._start;
    _stop = rhs._stop;
    _openStart = rhs._openStart;
    _openStop = rhs._openStop;
    return *this;
  }

  T start( ) const{
    return _start;
  }

  T stop( ) const{
    return _stop;
  }

  bool isOpenStart( ) const{
    return _openStart;
  }

  bool isOpenStop( ) const{
    return _openStop;
  }

  TInterval<T> unite(const TInterval<T>& other){
    T start, stop;
    bool openStart, openStop;
    if( _start < other._start){
      start = _start;
      openStart = _openStart;
    }else{
      start = other._start;
      openStart = other._openStart;
    }
    if( _stop > other._stop ){
      stop = _stop;
      openStop = _openStop;
    }else{
      stop = other._stop;
      openStop = other._openStop;
    }
    TInterval<T> rv(start, stop, openStart, openStop);
    return rv;
  }

  TInterval<T> intersect(const TInterval<T>& other){
    T start, stop;
    bool openStart, openStop;
    if( _start > other._start){
      start = _start;
      openStart = _openStart;
    }else{
      start = other._start;
      openStart = other._openStart;
    }
    if( _stop < other._stop ){
      stop = _stop;
      openStop = _openStop;
    }else{
      stop = other._stop;
      openStop = other._openStop;
    }
    TInterval<T> rv(start, stop, openStart, openStop);
    return rv;
  }


private:
  T _start;
  T _stop;
  bool _openStart;
  bool _openStop;
};

typedef TInterval<float> Interval;
typedef TInterval<double> Intervald;

template <typename T>
class TRay{
public:
  TRay( ){};
  
  TRay(const TPoint3<T>& origin, const TVec3<T>& direction):
    _origin(origin){
      _direction = normalize(direction);
    }
  
  TPoint3<T> origin( ) const {
    return _origin;
  }
  
  TVec3<T> direction( ) const {
    return _direction;
  }

  TPoint3<T> evaluate(T t) const {
    TPoint3<T> p = _origin + (t * _direction);
    return p;
  }
  
  std::ostream& write(std::ostream &out) const{
    out << "Ray(";
    _origin.write_row(out);
    out << ", ";
    _direction.write_row(out);
    out << ")" << std::endl;
    return out;
  }

private:
  TPoint3<T> _origin;
  TVec3<T> _direction;
};

template <typename T>
std::ostream& operator <<( std::ostream &out, const TRay<T> &r ){
  return r.write(out);
}

typedef TRay<float> Ray;
typedef TRay<double> Rayd;
typedef TRay<int> Rayi;

template <typename T>
class TBoundingSphere{
public:
  TBoundingSphere(TPoint3<T>& center, T radius): _center(center), _radius(radius) { };
  TBoundingSphere(TVec3<T>& center, T radius): _center(center), _radius(radius) { };
  TPoint3<T> center( ){
    return _center;
  }
  T radius( ){
    return _radius;
  }
  bool intersectWith(TRay<T>& ray){
    bool rv = false;
    float a, b, c, discriminant, dist, q, t0, t1, tmp;
    TVec3<T> o_minus_c;
    o_minus_c = ray.origin( ) - _center;
    a = dot(ray.direction( ), ray.direction( ));
    b = 2.0 * dot(ray.direction( ), o_minus_c);
    c = dot(o_minus_c, o_minus_c) - sqr(_radius);
    discriminant = b * b - 4.0 * a * c;
    if(discriminant > 0.0){
      dist = sqrtf(discriminant);
      if(b < 0.0){
        q = (-b - dist) / 2.0;
      }else{
        q = (-b + dist) / 2.0;
      }
      t0 = q / a;
      t1 = c / q;
      if(t0 > t1){
        tmp = t0;
        t0 = t1;
        t1 = tmp;
      }
      if(t1 < 0.0f){
        rv = false;
      }
      if(t0 < 0.0f){
        //h->t = t1;
        //rayEval3f(h->point, r, t1);
        rv = true;
      }else{
        //h->t = t0;
        //rayEval3f(h->point, r, t0);
        rv = true;
      }
    }
    return rv;
  }

  std::ostream& write(std::ostream &out) const{
    out << "BoundingSphere(";
    _center.write_row(out);
    out << ", ";
    out << _radius;
    out << ")" << std::endl;
    return out;
  }
private:
  TPoint3<T> _center;
  T _radius;
};

template <typename T>
std::ostream& operator <<( std::ostream &out, const TBoundingSphere<T> &s ){
  return s.write(out);
}

typedef TBoundingSphere<float> BoundingSphere;
typedef TBoundingSphere<double> BoundingSphered;
typedef TBoundingSphere<int> BoundingSpherei;

class GLViewPort{
public:
  GLViewPort( ){
    int viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    _origin_x = viewport[0];
    _origin_y = viewport[1];
    _width = viewport[2];
    _height= viewport[3];
  }

  int width( ){
    return _width;
  }

  int height( ){
    return _height;
  }

  float aspect( ){
    return float(_width) / float(_height);
  }
  
  double aspectd( ){
    return double(_width) / double(_height);
  }
  
  int originX( ){
    return _origin_x;
  }

  int originY( ){
    return _origin_y;
  }
private:
  int _width, _height, _origin_x, _origin_y;
};

template <typename T>
struct TQuadFloat{
#ifdef GFX_BIG_ENDIAN_BYTE_ORDER
      T red;
      T green;
      T blue;
      T alpha;
#else
      T blue;
      T green;
      T red;
      T alpha;
#endif
};

typedef TQuadFloat<float> QuadFloat;
typedef TQuadFloat<double> QuadDouble;

template <typename T>
union TChannels{
  TQuadFloat<T> quad;
  T all[4];
};

typedef TChannels<float> Channels;
typedef TChannels<double> Channelsd;

template <typename T>
class TRGBAColor{
public:
  TRGBAColor(T red = 0, T green = 0, T blue = 0, T alpha = 0){
    channels.quad.red = red;
    channels.quad.green = green;
    channels.quad.blue = blue;
    channels.quad.alpha = alpha;
/*#ifdef GFX_BIG_ENDIAN_BYTE_ORDER
  quad.red(red), quad.green(green), quad.blue(blue), quad.alpha(alpha)
#else
  quad.blue(blue), quad.green(green), quad.red(red), quad.alpha(alpha)
#endif*/
  }

  TRGBAColor(T* c){
    for(int i = 0; i < length; i++){
      channels.all[i] = c[i];
    }
  }
/*#ifdef GFX_BIG_ENDIAN_BYTE_ORDER
  quad.red(c[0]), quad.green(c[1]), quad.blue(c[2]), quad.alpha(c[3]){};
#else
  quad.blue(c[0]), quad.green(c[1]), quad.red(c[2]), quad.alpha(c[3]){};
#endif*/
  
  ~TRGBAColor( ){};
  
  TRGBAColor& operator =(const TRGBAColor& rhs){
    for(int i = 0; i < length; i++){
      channels.all[i] = rhs.channels.all[i];
    }
    return *this;
  }
  

  TRGBAColor operator +(const TRGBAColor& rhs) const{
    TRGBAColor rv;
    for( int i = 0; i < length; i++ ){
      rv.channels.all[i] = std::max(std::min(channels.all[i] + rhs.channels.all[i], T(1)), T(0));
      //rv.channels.all[i] = channels.all[i] + rhs.channels.all[i];
    }
    return rv;
  }
    
  TRGBAColor operator -(const TRGBAColor& rhs) const{
    TRGBAColor rv;
    for( int i = 0; i < length; i++ ){
      rv.channels.all[i] = std::max(std::min(channels.all[i] - rhs.channels.all[i], T(1)), T(0));
    }
    return rv;
  }

  TRGBAColor operator *(const TRGBAColor& rhs) const{
    TRGBAColor rv;
    for(int i = 0; i < length; i++){
      rv.channels.all[i] = std::max(std::min(channels.all[i] * rhs.channels.all[i], T(1)), T(0));
    }
    return rv;
  }
    
  TRGBAColor operator *(const T& s) const{
    //assert(s >= 0);
    TRGBAColor rv;
    for(int i = 0; i < length-1; i++){
      rv.channels.all[i] = std::max(std::min(channels.all[i] * s, T(1)), T(0));
    }
    rv.channels.quad.alpha = channels.quad.alpha;
    return rv;
  }
  
  TRGBAColor& operator *=(const T& s){
    assert(s >= 0);
    for(int i = 0; i < length-1; i++){
      channels.all[i] = std::max(std::min(channels.all[i] * s, T(1)), T(0));
    }

    return *this;
  }
  
  TRGBAColor operator /(const TRGBAColor& rhs) const{
    TRGBAColor rv;
    for(int i = 0; i < length-1; i++){
      rv.channels.all[i] = std::max(std::min(channels.all[i] * rhs.channels.all[i], T(1)), T(0));
    }
  }

  TRGBAColor operator /(const T& s) const{
    TRGBAColor rv;
    assert(s > 0);
    for( int i = 0; i < length-1; i++ ){
      rv.channels.all[i] = std::max(std::min(channels.all[i] / s, T(1)), T(0));
    }
    rv.channels.quad.alpha = channels.quad.alpha;
    return rv;
  }
  
  TRGBAColor operator /=(const T& s){
    for(int i = 0; i < length-1; i++){
      channels.all[i] = std::max(std::min(channels.all[i] / s, T(1)), T(0));
    }
    return *this;
  }

  T& operator [](unsigned int i){
    if( i < length ){
      return channels.all[i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  const T& operator [](unsigned int i) const{
    if( i < length ){
      return channels.all[i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  bool operator ==(const TRGBAColor& rhs) const{
    bool rv = false;
    for( int i = 0; i < length; i++ ){
      rv = (channels.all[i] == rhs.channels.all[i]);
      if(!rv)
        break;
    }
    return rv;
  }
  
  bool operator !=(const TRGBAColor& rhs) const{
    bool rv = false;
    for( int i = 0; i < length; i++ ){
      rv = (channels.all[i] != rhs.channels.all[i]);
      if(!rv)
        break;
    }
    return rv;
  }
    
  operator T*( ){
    return &channels.all[0];
  }

  operator const T*( ) const{
    return &channels.all[0];
  }
  
  void copyToRGBQuad(RGBQUAD& q) const{
    q.rgbRed = static_cast<unsigned char>(std::round(255 * channels.quad.red));
    q.rgbGreen = static_cast<unsigned char>(std::round(255 * channels.quad.green));
    q.rgbBlue = static_cast<unsigned char>(std::round(255 * channels.quad.blue));
    q.rgbReserved = static_cast<unsigned char>(std::round(255 * channels.quad.alpha));
  }

  unsigned char redByte( ) const{
    return static_cast<unsigned char>(std::round(255 * channels.quad.red));
  }

  unsigned char greenByte( ) const{
    return static_cast<unsigned char>(std::round(255 * channels.quad.green));
  }

  unsigned char blueByte( ) const{
    return static_cast<unsigned char>(std::round(255 * channels.quad.blue));
  }

  unsigned char alphaByte( ) const{
    return static_cast<unsigned char>(std::round(255 * channels.quad.alpha));
  }

  std::ostream& write( std::ostream &out ) const{
    out << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << " " << channels.all[i] << std::endl;
      }else{
        out << " " << channels.all[i] << " )" << std::endl;
      }
    }
    return( out );
  }
  
  std::ostream& write_row( std::ostream &out) const{
    out << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << channels.all[i] << ", ";
      }else{
        out << channels.all[i] << ")";
      }
    }
    return( out );
  }
  
  std::ostream& description( std::ostream &out = std::cerr ) const{
    out << "<TRGBAColor"<< length << " " << this << "> ";
    out << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << channels.all[i] << ", ";
      }else{
        out << channels.all[i] << ")" << std::endl;
      }
    }
    return( out );
  }

//private:
  const static size_t length = 4;
  TChannels<T> channels;
};

typedef TRGBAColor<float> RGBAColor;
typedef TRGBAColor<double> RGBAColord;

template <typename T>
std::ostream& operator <<( std::ostream &out, const TRGBAColor<T> &c ){
  return(c.write_row(out));
}


template <typename T>
class TQuadric{
public:
  TQuadric( ){};
};

typedef TQuadric<float> Quadric;
typedef TQuadric<double> Quadricd;

template <typename T>
class TQuaternion{
public:
  TQuaternion( ) : _s(0), _v( ) { }
  TQuaternion(T s, TVec3<T>& v) : _s(s), _v(v) { }
  TQuaternion(T s, T i, T j, T k) : _s(s), _v(i, j, k) { }
  
  TQuaternion& operator =(const TQuaternion& rhs){
    _s = rhs._s;
    _v = rhs._v;
    return *this;
  }
  
  TQuaternion operator +(const TQuaternion& rhs) const{
    TQuaternion sum(_s + rhs._s, _v + rhs._v);
    return sum;
  }
  
  TQuaternion& operator +=(const TQuaternion& rhs){
    return *this = *this + rhs;
  }

  TQuaternion operator -( ) const{
    TQuaternion rv(-_s, -_v);
    return rv;
  }
  
  TQuaternion operator -(const TQuaternion& rhs) const{
    TQuaternion difference(_s - rhs._s, _v - rhs._v);
    return difference;
  }
  
  TQuaternion& operator -=(const TQuaternion& rhs){
    return *this = *this - rhs;
  }
  
  /*
  TQuaternion operator *(const TQuaternion& rhs) const{
    T s = (_s * rhs._s) - dot(_v, rhs._v);
    assert(0 == 1);
    TVec3<T> v(_s * rhs._v) + (rhs._s * _v) + cross(_v, rhs._v);
    return TQuaternion(s, v);
  }*/
  
  TQuaternion operator /(const T& s) const{
    return TQuaternion(_s / s, _v / s);
  }
  
  TQuaternion conjugate( ){
    return TQuaternion(_s, -_v);
  }
  
  T squarednorm( ){
    return sqr(_s) + dot(_v, _v);
  }
  
  T norm( ){
    return sqrt(squarednorm( ));
  }
  
  /*
  TQuaternion normalize( ){
    n = norm( );
    assert(0 == 1);
    return TQuaternion(_s/n, _v/n);
  }
  */
  
  TQuaternion inverse( ){
    return conjugate( ) / squarednorm( );
  }
  
  T& operator [](unsigned int i){
    if( i == 0){
      return _s;
    }else if(i < 4){
      return _v[(i-1)];
    }else{
      throw( "Index out of range" );
    }
  }
  
  const T& operator [](unsigned int i) const{
    if( i == 0){
      return _s;
    }else if(i < 4){
      return _v[(i-1)];
    }else{
      throw( "Index out of range" );
    }
  }
  
  bool operator ==(const TQuaternion& rhs) const{
    bool rv = false;
    float precision = FP_SP_EPSILON;
    if(_s == rhs._s){
      for( int i = 0; i < _v.size( ); i++ ){
        rv = fpEqual(_v[i], rhs._v[i], precision);
        if(!rv)
          break;
      }
    }
    return rv;
  }
  
  bool operator !=(const TQuaternion& rhs) const{
    bool rv = false;
    float precision = FP_SP_EPSILON;
    if(_s != rhs._s){
      for( int i = 0; i < _v.size( ); i++ ){
        rv = fpNotEqual(_v[i], rhs._v[i], precision);
        if(!rv)
          break;
      }
    }
    return rv;
  }
    
  /*
  std::ostream& write( std::ostream &out ) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION)
      << "(" << _s << ", ";
    for( int i = 0; i < _v.size( ); i++ ){
      if( i < (_v.size( ) - 1) ){
        out << " " << data[i] << std::endl;
      }else{
        out << " " << data[i] << " )" << std::endl;
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  */
  
private:
  T _s;
  TVec3<T> _v;
};

/*
template <typename T>
static T dot(const TQuaternion<T>& a, const TQuaternion<T>& b){
  T rv = T(0);
  for( int i = 0; i < 4; i++){
    rv += a[i] * b[i];
  }
  return rv;
}

template <typename T>
static TQuaternion slerp(const T& t, const TQuaternion<T>& a, const TQuaternion<T>& b){
  assert(false);
}

template <typename T>
static TQuaternion squad(const T& t, const TQuaternion<T>& p, const TQuaternion<T>& a, const TQuaternion<T>& b, const TQuaternion<T>& q){
  assert(false);
}
*/

typedef TQuaternion<float> Quaternion;
typedef TQuaternion<double> Quaterniond;


static bool project(const Vec3& objCoord, const Mat4& projection, const Mat4& modeling, GLViewPort& vp, Vec3* winCoord){
  assert(false);
  /*float precision = FP_SP_EPSILON;
  Vec4 in(objCoord, 1.0);
  Vec4 out = modeling * in;
  out = projection * out;
  if(fpEqual(out[3], 0.0f, precision)){
    return false;
  }
  out[0] /= out[3];
  out[1] /= out[3];
  out[2] /= out[3];

  // Map x, y and z to range 0-1
  out[0] = (out[0] * 0.5) + 0.5;
  out[1] = (out[1] * 0.5) + 0.5;
  out[2] = (out[2] * 0.5) + 0.5;
  
  // Map x,y to viewport
  out[0] = out[0] * viewport[2] + viewport[0];
  out[1] = out[1] * viewport[3] + viewport[1];
  
  *winx=in[0];
  *winy=in[1];
  *winz=in[2];*/
  return(true);
}

static bool unproject(const Vec3& winCoord, const Mat4& projection, const Mat4& modeling, GLViewPort& vp, Vec3& objCoord){
  bool rv = false;
  float precision = FP_SP_EPSILON;

  Mat4 finalMatrix;
  // Why transpose??? Perhaps I have a gross error in my
  // matrix class that I never noticed.
  finalMatrix = (projection * modeling).transpose( );
  Mat4 finalMatrixInverse = finalMatrix.inverse( );

  /*std::cerr << "projection" << std::endl;
  std::cerr << projection << std::endl;

  std::cerr << "modeling" << std::endl;
  std::cerr << modeling << std::endl;

  std::cerr << "final" << std::endl;
  std::cerr << finalMatrix << std::endl;

  std::cerr << "final inverse" << std::endl;
  std::cerr << finalMatrixInverse << std::endl;*/

  Vec4 in(winCoord, 1.0);
  // Map x and y from window coordinates
  in[0] = (in[0] - vp.originX( )) / vp.width( );
  in[1] = (in[1] - vp.originY( )) / vp.height( );

  // Map to range -1 to 1
  in[0] = in[0] * 2 - 1;
  in[1] = in[1] * 2 - 1;
  in[2] = in[2] * 2 - 1;
  //in *= 2.0;
  //in -= 1.0;

  //std::cerr << "in is: " << in << std::endl;

  Vec4 out = finalMatrixInverse * in;

  if(fpNotEqual(out[3], 0.0f, precision)){
    out /= out[3];
    objCoord = Vec3(out[0], out[1], out[2]);
    rv = true;
  }else{
    std::cerr << "It's zero" << std::endl;
  }
  return rv;
}


#endif
