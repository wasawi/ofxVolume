#pragma once

#include "ofVec2f.h"
#include "ofVec4f.h"
#include "ofConstants.h"
#include "ofUtils.h"

#include <cmath>
#include <iostream>

template < typename Type >
class ofxPoint_ {
public:
	Type x,y,z;
    
    static const int DIM = 3;
	
	ofxPoint_();
	ofxPoint_( Type _x, Type _y, Type _z=0 );
	/// assigns scalar to x, y, and z
	explicit ofxPoint_( Type _scalar );
	
    ofxPoint_( const ofVec2f& vec );
    ofxPoint_( const ofVec4f& vec );
	
	Type * getPtr() {
		return (Type*)&x;
	}
	const Type * getPtr() const {
		return (const Type *)&x;
	}
	
	Type& operator[]( int n ){
		return getPtr()[n];
	}
	
	Type operator[]( int n ) const {
		return getPtr()[n];
	}
	
    
	// Getters and Setters.
    //
    void set( Type _x, Type _y, Type _z = 0 );
    void set( const ofxPoint_& vec );
	void set( Type _scalar );
	
    // Check similarity/equality.
    //
    bool operator==( const ofxPoint_& vec ) const;
    bool operator!=( const ofxPoint_& vec ) const;
    bool match( const ofxPoint_& vec, Type tolerance=0.0001 ) const;
    /**
	 * Checks if vectors look in the same direction.
	 */
    bool isAligned( const ofxPoint_& vec, Type tolerance=0.0001 ) const;
    bool align( const ofxPoint_& vec, Type tolerance=0.0001 ) const;
    bool isAlignedRad( const ofxPoint_& vec, Type tolerance=0.0001 ) const;
    bool alignRad( const ofxPoint_& vec, Type tolerance=0.0001 ) const;
	
	
    // Operator overloading for ofxPoint_
    //
    ofxPoint_  operator+( const ofxPoint_& pnt ) const;
    ofxPoint_& operator+=( const ofxPoint_& pnt );
    ofxPoint_  operator-( const ofxPoint_& vec ) const;
    ofxPoint_& operator-=( const ofxPoint_& vec );
    ofxPoint_  operator*( const ofxPoint_& vec ) const;
    ofxPoint_& operator*=( const ofxPoint_& vec );
    ofxPoint_  operator/( const ofxPoint_& vec ) const;
    ofxPoint_& operator/=( const ofxPoint_& vec );
    ofxPoint_  operator-() const;
	
    //operator overloading for Type
    //
	//    void 	  operator=( const Type f);
    ofxPoint_  operator+( const Type f ) const;
    ofxPoint_& operator+=( const Type f );
 	ofxPoint_  operator-( const Type f ) const;
    ofxPoint_& operator-=( const Type f );
    ofxPoint_  operator*( const Type f ) const;
    ofxPoint_& operator*=( const Type f );
    ofxPoint_  operator/( const Type f ) const;
    ofxPoint_& operator/=( const Type f );
	
	
	// Operator overloading for ofxPoint_
	//
	//
//	template < typename S >	friend ostream& operator<<(ostream& os, const ofxPoint_<Type> vec);
//	template < typename S >	friend istream& operator>>(istream& is, ofxPoint_<S>& vec);
	
	friend std::ostream& operator<<( std::ostream& lhs, const ofxPoint_<Type>& rhs )
	{
		lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "]";
		return lhs;
	}

	
    //Scale
    //
    ofxPoint_  getScaled( const Type length ) const;
    ofxPoint_& scale( const Type length );
    
	
    // Rotation
    //
    ofxPoint_  getRotated( Type angle, const ofxPoint_& axis ) const;
    ofxPoint_  getRotatedRad( Type angle, const ofxPoint_& axis ) const;
    ofxPoint_& rotate( Type angle, const ofxPoint_& axis );
    ofxPoint_& rotateRad( Type angle, const ofxPoint_& axis );
    ofxPoint_  getRotated(Type ax, Type ay, Type az) const;
    ofxPoint_  getRotatedRad(Type ax, Type ay, Type az) const;
    ofxPoint_& rotate(Type ax, Type ay, Type az);
    ofxPoint_& rotateRad(Type ax, Type ay, Type az);
    
    
    // Rotation - point around pivot
    //    
    ofxPoint_   getRotated( Type angle, const ofxPoint_& pivot, const ofxPoint_& axis ) const;
    ofxPoint_&  rotate( Type angle, const ofxPoint_& pivot, const ofxPoint_& axis );
    ofxPoint_   getRotatedRad( Type angle, const ofxPoint_& pivot, const ofxPoint_& axis ) const;
    ofxPoint_&  rotateRad( Type angle, const ofxPoint_& pivot, const ofxPoint_& axis );    
	
	
    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    ofxPoint_ getMapped( const ofxPoint_& origin,
					  const ofxPoint_& vx,
					  const ofxPoint_& vy,
					  const ofxPoint_& vz ) const;
    ofxPoint_& map( const ofxPoint_& origin,
				 const ofxPoint_& vx,
				 const ofxPoint_& vy,
				 const ofxPoint_& vz );
	
	
    // Distance between two points.
    //
    Type distance( const ofxPoint_& pnt) const;
    Type squareDistance( const ofxPoint_& pnt ) const;
	
	
    // Linear interpolation.
    //
    /**
	 * p==0.0 results in this point, p==0.5 results in the
	 * midpoint, and p==1.0 results in pnt being returned.
	 */
    ofxPoint_   getInterpolated( const ofxPoint_& pnt, Type p ) const;
    ofxPoint_&  interpolate( const ofxPoint_& pnt, Type p );
    ofxPoint_   getMiddle( const ofxPoint_& pnt ) const;
    ofxPoint_&  middle( const ofxPoint_& pnt );
    ofxPoint_&  average( const ofxPoint_* points, int num );
    
	
    // Normalization
    //
    ofxPoint_  getNormalized() const;
    ofxPoint_& normalize();
	
	
    // Limit length.
    //
    ofxPoint_  getLimited(Type max) const;
    ofxPoint_& limit(Type max);
	
	
    // Perpendicular vector.
    //
    ofxPoint_  getCrossed( const ofxPoint_& vec ) const;
    ofxPoint_& cross( const ofxPoint_& vec );
    /**
	 * Normalized perpendicular.
	 */
    ofxPoint_  getPerpendicular( const ofxPoint_& vec ) const;
    ofxPoint_& perpendicular( const ofxPoint_& vec );
	
	
    // Length
    //
    Type length() const;
    Type lengthSquared() const;

    /**
	 * Angle (deg) between two vectors.
	 * This is an unsigned relative angle from 0 to 180.
	 * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
	 */
    Type angle( const ofxPoint_& vec ) const;
    Type angleRad( const ofxPoint_& vec ) const;
	
	
    // Dot Product
    //
    Type dot( const ofxPoint_& vec ) const;
	
	
	
    //-----------------------------------------------
    // this methods are deprecated in 006 please use:
	
    // getScaled
    ofxPoint_ rescaled( const Type length ) const;
	
    // scale
    ofxPoint_& rescale( const Type length );
	
    // getRotated
    ofxPoint_ rotated( Type angle, const ofxPoint_& axis ) const;
	
    // getRotated should this be const???
    ofxPoint_ rotated(Type ax, Type ay, Type az);
	
    // getNormalized
    ofxPoint_ normalized() const;
	
    // getLimited
    ofxPoint_ limited(Type max) const;
	
    // getCrossed
    ofxPoint_ crossed( const ofxPoint_& vec ) const;
	
    // getPerpendicular
    ofxPoint_ perpendiculared( const ofxPoint_& vec ) const;
    
    // use getMapped
    ofxPoint_  mapped( const ofxPoint_& origin,
					const ofxPoint_& vx,
					const ofxPoint_& vy,
					const ofxPoint_& vz ) const;
	
    // use squareDistance
    Type  distanceSquared( const ofxPoint_& pnt ) const;
	
    // use getInterpolated
    ofxPoint_ 	interpolated( const ofxPoint_& pnt, Type p ) const;
	
    // use getMiddle
    ofxPoint_ 	middled( const ofxPoint_& pnt ) const;
    
    // use getRotated
    ofxPoint_ 	rotated( Type angle,
						const ofxPoint_& pivot,
						const ofxPoint_& axis ) const;    

    // return all zero vector
    static ofxPoint_ zero() { return ofxPoint_(0, 0, 0); }
    
    // return all one vector
    static ofxPoint_ one() { return ofxPoint_(1, 1, 1); }
};

template class ofxPoint_<char>;
template class ofxPoint_<unsigned char>;
template class ofxPoint_<short>;
template class ofxPoint_<unsigned short>;
template class ofxPoint_<int>;
template class ofxPoint_<unsigned int>;
template class ofxPoint_<long>;
template class ofxPoint_<unsigned long>;
template class ofxPoint_<float>;
template class ofxPoint_<double>;

typedef ofxPoint_<int> ofxPoint;
typedef ofxPoint_<char> ofxCharPoint;
typedef ofxPoint_<unsigned char> ofxUCharPoint;
typedef ofxPoint_<short> ofxShortPoint;
typedef ofxPoint_<unsigned short> ofxUShortPoint;
typedef ofxPoint_<int> ofxIntPoint;
typedef ofxPoint_<unsigned int> ofxUIntPoint;
typedef ofxPoint_<long> ofxLongPoint;
typedef ofxPoint_<unsigned long> ofxULongPoint;
typedef ofxPoint_<float> ofxFloatPoint;
typedef ofxPoint_<double> ofxDoublePoint;

typedef ofxPoint& ofxPointRef;
typedef ofxCharPoint& ofxCharPointRef;
typedef ofxUCharPoint& ofxUCharPointRef;
typedef ofxShortPoint& ofxShortPointRef;
typedef ofxUShortPoint& ofxUShortPointRef;
typedef ofxIntPoint& ofxIntPointRef;
typedef ofxUIntPoint& ofxUIntPointRef;
typedef ofxLongPoint& ofxLongPointRef;
typedef ofxULongPoint& ofxULongPointRef;
typedef ofxFloatPoint& ofFloatPointRef;
typedef ofxDoublePoint& ofxDoublePointRef;


// Non-Member operators
//
//
template < typename Type >
ofxPoint_<Type> operator+( Type f, const ofxPoint_<Type>& vec );
template < typename Type >
ofxPoint_<Type> operator-( Type f, const ofxPoint_<Type>& vec );
template < typename Type >
ofxPoint_<Type> operator*( Type f, const ofxPoint_<Type>& vec );
template < typename Type >
ofxPoint_<Type> operator/( Type f, const ofxPoint_<Type>& vec );


/////////////////
// Implementation
/////////////////

template < typename Type >
inline ofxPoint_<Type>::ofxPoint_( const ofVec2f& vec ):x(vec.x), y(vec.y), z(0) {}
template < typename Type >
inline ofxPoint_<Type>::ofxPoint_( const ofVec4f& vec ):x(vec.x), y(vec.y), z(vec.z) {}
template < typename Type >
inline ofxPoint_<Type>::ofxPoint_(): x(0), y(0), z(0) {};
template < typename Type >
inline ofxPoint_<Type>::ofxPoint_( Type _all ): x(_all), y(_all), z(_all) {};
template < typename Type >
inline ofxPoint_<Type>::ofxPoint_( Type _x, Type _y, Type _z ):x(_x), y(_y), z(_z) {}


// Getters and Setters.
//
//
template < typename Type >
inline void ofxPoint_<Type>::set( Type _scalar ) {
	x = _scalar;
	y = _scalar;
	z = _scalar;
}
template < typename Type >
inline void ofxPoint_<Type>::set( Type _x, Type _y, Type _z ) {
	x = _x;
	y = _y;
	z = _z;
}
template < typename Type >
inline void ofxPoint_<Type>::set( const ofxPoint_& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


// Check similarity/equality.
//
//
template < typename Type >
inline bool ofxPoint_<Type>::operator==( const ofxPoint_& vec ) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}
template < typename Type >
inline bool ofxPoint_<Type>::operator!=( const ofxPoint_& vec ) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}
template < typename Type >
inline bool ofxPoint_<Type>::match( const ofxPoint_& vec, Type tolerance ) const{
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance)
	&& (fabs(z - vec.z) < tolerance);
}

/**
 * Checks if vectors look in the same direction.
 */
template < typename Type >
inline bool ofxPoint_<Type>::isAligned( const ofxPoint_& vec, Type tolerance ) const {
	Type angle = this->angle( vec );
	return  angle < tolerance;
}
template < typename Type >
inline bool ofxPoint_<Type>::align( const ofxPoint_& vec, Type tolerance ) const {
    return isAligned( vec, tolerance );
}
template < typename Type >
inline bool ofxPoint_<Type>::isAlignedRad( const ofxPoint_& vec, Type tolerance ) const {
	Type angle = this->angleRad( vec );
	return  angle < tolerance;
}
template < typename Type >
inline bool ofxPoint_<Type>::alignRad( const ofxPoint_& vec, Type tolerance ) const {
    return isAlignedRad( vec, tolerance );
}


// Operator overloading for ofxPoint_
//
//
/*template < typename S >
ostream& operator<<( ostream& os, const ofxPoint_<Type>& vec ) {
//	os << "[" << ofToString(vec.x) << "," << ofToString(vec.y) << "," << ofToString(vec.z) << "]";
	os << "[" << vec.x << "," << vec.y << "," << vec.z << "]";

	return os;
}
template < typename S >
inline istream& operator>>( istream& is, ofxPoint_<S>& vec ) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	return is;
}*/

template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator+( const ofxPoint_& pnt ) const {
	return ofxPoint_( x+pnt.x, y+pnt.y, z+pnt.z );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator+=( const ofxPoint_& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator-( const ofxPoint_& vec ) const {
	return ofxPoint_( x-vec.x, y-vec.y, z-vec.z );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator-=( const ofxPoint_& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator*( const ofxPoint_& vec ) const {
	return ofxPoint_( x*vec.x, y*vec.y, z*vec.z );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator*=( const ofxPoint_& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator/( const ofxPoint_& vec ) const {
	return ofxPoint_( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator/=( const ofxPoint_& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator-() const {
	return ofxPoint_( -x, -y, -z );
}


//operator overloading for Type
//
//
//inline void ofxPoint_<Type>::operator=( const Type f){
//	x = f;
//	y = f;
//	z = f;
//}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator+( const Type f ) const {
	return ofxPoint_( x+f, y+f, z+f);
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator+=( const Type f ) {
	x += f;
	y += f;
	z += f;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator-( const Type f ) const {
	return ofxPoint_( x-f, y-f, z-f);
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator-=( const Type f ) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator*( const Type f ) const {
	return ofxPoint_( x*f, y*f, z*f );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator*=( const Type f ) {
	x*=f;
	y*=f;
	z*=f;
	return *this;
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::operator/( const Type f ) const {
	if(f == 0) return ofxPoint_( x, y, z);
	
	return ofxPoint_( x/f, y/f, z/f );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::operator/=( const Type f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	z/=f;
	return *this;
}


//Scale
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::rescaled( const Type length ) const {
	return getScaled(length);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getScaled( const Type length ) const {
	Type l = (Type)sqrt(x*x + y*y + z*z);
	if( l > 0 )
		return ofxPoint_( (x/l)*length, (y/l)*length, (z/l)*length );
	else
		return ofxPoint_();
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rescale( const Type length ) {
	return scale(length);
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::scale( const Type length ) {
	Type l = (Type)sqrt(x*x + y*y + z*z);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
	}
	return *this;
}



// Rotation
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::rotated( Type angle, const ofxPoint_& axis ) const {
	return getRotated(angle, axis);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getRotated( Type angle, const ofxPoint_& axis ) const {
	ofxPoint_ ax = axis.normalized();
	Type a = (Type)(angle*DEG_TO_RAD);
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	return ofxPoint_( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getRotatedRad( Type angle, const ofxPoint_& axis ) const {
	ofxPoint_ ax = axis.normalized();
	Type a = angle;
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	return ofxPoint_( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rotate( Type angle, const ofxPoint_& axis ) {
	ofxPoint_ ax = axis.normalized();
	Type a = (Type)(angle*DEG_TO_RAD);
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	Type nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	Type ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	Type nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rotateRad(Type angle, const ofxPoint_& axis ) {
	ofxPoint_ ax = axis.normalized();
	Type a = angle;
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	Type nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	Type ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	Type nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

// const???
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::rotated(Type ax, Type ay, Type az) {
	return getRotated(ax,ay,az);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getRotated(Type ax, Type ay, Type az) const {
	Type a = (Type)cos(DEG_TO_RAD*(ax));
	Type b = (Type)sin(DEG_TO_RAD*(ax));
	Type c = (Type)cos(DEG_TO_RAD*(ay));
	Type d = (Type)sin(DEG_TO_RAD*(ay));
	Type e = (Type)cos(DEG_TO_RAD*(az));
	Type f = (Type)sin(DEG_TO_RAD*(az));
	
	Type nx = c * e * x - c * f * y + d * z;
	Type ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	Type nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return ofxPoint_( nx, ny, nz );
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getRotatedRad(Type ax, Type ay, Type az) const {
	Type a = cos(ax);
	Type b = sin(ax);
	Type c = cos(ay);
	Type d = sin(ay);
	Type e = cos(az);
	Type f = sin(az);
	
	Type nx = c * e * x - c * f * y + d * z;
	Type ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	Type nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return ofxPoint_( nx, ny, nz );
}

template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rotate(Type ax, Type ay, Type az) {
	Type a = (Type)cos(DEG_TO_RAD*(ax));
	Type b = (Type)sin(DEG_TO_RAD*(ax));
	Type c = (Type)cos(DEG_TO_RAD*(ay));
	Type d = (Type)sin(DEG_TO_RAD*(ay));
	Type e = (Type)cos(DEG_TO_RAD*(az));
	Type f = (Type)sin(DEG_TO_RAD*(az));
	
	Type nx = c * e * x - c * f * y + d * z;
	Type ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	Type nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}

template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rotateRad(Type ax, Type ay, Type az) {
	Type a = cos(ax);
	Type b = sin(ax);
	Type c = cos(ay);
	Type d = sin(ay);
	Type e = cos(az);
	Type f = sin(az);
	
	Type nx = c * e * x - c * f * y + d * z;
	Type ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	Type nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}


// Rotate point by angle (deg) around line defined by pivot and axis.
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::rotated( Type angle,
								const ofxPoint_& pivot,
								const ofxPoint_& axis ) const{
	return getRotated(angle, pivot, axis);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getRotated( Type angle,
								   const ofxPoint_& pivot,
								   const ofxPoint_& axis ) const
{
	ofxPoint_ ax = axis.normalized();
	Type tx = x - pivot.x;
	Type ty = y - pivot.y;
	Type tz = z - pivot.z;
	
	Type a = (Type)(angle*DEG_TO_RAD);
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	Type xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	Type yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	Type zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return ofxPoint_( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}

template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getRotatedRad( Type angle,
									  const ofxPoint_& pivot,
									  const ofxPoint_& axis ) const
{
	ofxPoint_ ax = axis.normalized();
	Type tx = x - pivot.x;
	Type ty = y - pivot.y;
	Type tz = z - pivot.z;
	
	Type a = angle;
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	Type xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	Type yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	Type zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return ofxPoint_( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}

template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rotate( Type angle,
								const ofxPoint_& pivot,
								const ofxPoint_& axis )
{
	ofxPoint_ ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	Type a = (Type)(angle*DEG_TO_RAD);
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	Type xrot = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	Type yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	Type zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}

template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::rotateRad( Type angle,
								   const ofxPoint_& pivot,
								   const ofxPoint_& axis )
{
	ofxPoint_ ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	Type a = angle;
	Type sina = sin( a );
	Type cosa = cos( a );
	Type cosb = 1.0f - cosa;
	
	Type xrot = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	Type yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	Type zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}




// Map point to coordinate system defined by origin, vx, vy, and vz.
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::mapped( const ofxPoint_& origin,
							   const ofxPoint_& vx,
							   const ofxPoint_& vy,
							   const ofxPoint_& vz ) const{
	return getMapped(origin, vx, vy, vz);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getMapped( const ofxPoint_& origin,
								  const ofxPoint_& vx,
								  const ofxPoint_& vy,
								  const ofxPoint_& vz ) const
{
	return ofxPoint_( origin.x + x*vx.x + y*vy.x + z*vz.x,
				   origin.y + x*vx.y + y*vy.y + z*vz.y,
				   origin.z + x*vx.z + y*vy.z + z*vz.z );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::map( const ofxPoint_& origin,
							 const ofxPoint_& vx,
							 const ofxPoint_& vy,
							 const ofxPoint_& vz )
{
	Type xmap = origin.x + x*vx.x + y*vy.x + z*vz.x;
	Type ymap =  origin.y + x*vx.y + y*vy.y + z*vz.y;
	z = origin.z + x*vx.z + y*vy.z + z*vz.z;
	x = xmap;
	y = ymap;
	return *this;
}


// Distance between two points.
//
//
template < typename Type >
inline Type ofxPoint_<Type>::distance( const ofxPoint_& pnt) const {
	Type vx = x-pnt.x;
	Type vy = y-pnt.y;
	Type vz = z-pnt.z;
	return (Type)sqrt(vx*vx + vy*vy + vz*vz);
}
template < typename Type >
inline Type  ofxPoint_<Type>::distanceSquared( const ofxPoint_& pnt ) const{
	return squareDistance(pnt);
}
template < typename Type >
inline Type  ofxPoint_<Type>::squareDistance( const ofxPoint_& pnt ) const {
	Type vx = x-pnt.x;
	Type vy = y-pnt.y;
	Type vz = z-pnt.z;
	return vx*vx + vy*vy + vz*vz;
}



// Linear interpolation.
//
//
/**
 * p==0.0 results in this point, p==0.5 results in the
 * midpoint, and p==1.0 results in pnt being returned.
 */
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::interpolated( const ofxPoint_& pnt, Type p ) const {
	return getInterpolated(pnt,p);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getInterpolated( const ofxPoint_& pnt, Type p ) const {
	return ofxPoint_( x*(1-p) + pnt.x*p,
				   y*(1-p) + pnt.y*p,
				   z*(1-p) + pnt.z*p );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::interpolate( const ofxPoint_& pnt, Type p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	return *this;
}

template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::middled( const ofxPoint_& pnt ) const {
	return getMiddle(pnt);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getMiddle( const ofxPoint_& pnt ) const {
	return ofxPoint_( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f, (z+pnt.z)/2.0f );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::middle( const ofxPoint_& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	return *this;
}


// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::average( const ofxPoint_* points, int num ) {
	x = 0.f;
	y = 0.f;
	z = 0.f;
	for( int i=0; i<num; i++) {
		x += points[i].x;
		y += points[i].y;
		z += points[i].z;
	}
	x /= num;
	y /= num;
	z /= num;
	return *this;
}



// Normalization
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::normalized() const {
	return getNormalized();
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getNormalized() const {
	Type length = (Type)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		return ofxPoint_( x/length, y/length, z/length );
	} else {
		return ofxPoint_();
	}
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::normalize() {
	Type length = (Type)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}



// Limit length.
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::limited(Type max) const {
	return getLimited(max);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getLimited(Type max) const {
    ofxPoint_ limited;
    Type lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        Type ratio = max/(Type)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio);
    } else {
        limited.set(x,y,z);
    }
    return limited;
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::limit(Type max) {
    Type lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        Type ratio = max/(Type)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }
    return *this;
}


// Perpendicular vector.
//
//
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::crossed( const ofxPoint_& vec ) const {
	return getCrossed(vec);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getCrossed( const ofxPoint_& vec ) const {
	return ofxPoint_( y*vec.z - z*vec.y,
				   z*vec.x - x*vec.z,
				   x*vec.y - y*vec.x );
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::cross( const ofxPoint_& vec ) {
	Type _x = y*vec.z - z*vec.y;
	Type _y = z*vec.x - x*vec.z;
	z = x*vec.y - y*vec.x;
	x = _x;
	y = _y;
	return *this;
}

/**
 * Normalized perpendicular.
 */
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::perpendiculared( const ofxPoint_& vec ) const {
	return getPerpendicular(vec);
}
template < typename Type >
inline ofxPoint_<Type> ofxPoint_<Type>::getPerpendicular( const ofxPoint_& vec ) const {
	Type crossX = y*vec.z - z*vec.y;
	Type crossY = z*vec.x - x*vec.z;
	Type crossZ = x*vec.y - y*vec.x;
	
	Type length = (Type)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 )
		return ofxPoint_( crossX/length, crossY/length, crossZ/length );
	else
		return ofxPoint_();
}
template < typename Type >
inline ofxPoint_<Type>& ofxPoint_<Type>::perpendicular( const ofxPoint_& vec ) {
	Type crossX = y*vec.z - z*vec.y;
	Type crossY = z*vec.x - x*vec.z;
	Type crossZ = x*vec.y - y*vec.x;
	
	Type length = (Type)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 ) {
		x = crossX/length;
		y = crossY/length;
		z = crossZ/length;
	} else {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	
	return *this;
}


// Length
//
//
template < typename Type >
inline Type ofxPoint_<Type>::length() const {
	return (Type)sqrt( x*x + y*y + z*z );
}
template < typename Type >
inline Type ofxPoint_<Type>::lengthSquared() const {
	return (Type)(x*x + y*y + z*z);
}



/**
 * Angle (deg) between two vectors.
 * This is an unsigned relative angle from 0 to 180.
 * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
 */
template < typename Type >
inline Type ofxPoint_<Type>::angle( const ofxPoint_& vec ) const {
	ofxPoint_ n1 = this->normalized();
	ofxPoint_ n2 = vec.normalized();
	return (Type)(acos( n1.dot(n2) )*RAD_TO_DEG);
}
template < typename Type >
inline Type ofxPoint_<Type>::angleRad( const ofxPoint_& vec ) const {
	ofxPoint_ n1 = this->normalized();
	ofxPoint_ n2 = vec.normalized();
	return (Type)acos( n1.dot(n2) );
}



/**
 * Dot Product.
 */
template < typename Type >
inline Type ofxPoint_<Type>::dot( const ofxPoint_& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z;
}





// Non-Member operators
//
//
template < typename Type >
inline ofxPoint_<Type> operator+( Type f, const ofxPoint_<Type>& vec ) {
    return ofxPoint_<Type>( f+vec.x, f+vec.y, f+vec.z );
}
template < typename Type >
inline ofxPoint_<Type> operator-( Type f, const ofxPoint_<Type>& vec ) {
    return ofxPoint_<Type>( f-vec.x, f-vec.y, f-vec.z );
}
template < typename Type >
inline ofxPoint_<Type> operator*( Type f, const ofxPoint_<Type>& vec ) {
    return ofxPoint_<Type>( f*vec.x, f*vec.y, f*vec.z );
}
template < typename Type >
inline ofxPoint_<Type> operator/( Type f, const ofxPoint_<Type>& vec ) {
    return ofxPoint_<Type>( f/vec.x, f/vec.y, f/vec.z);
}


