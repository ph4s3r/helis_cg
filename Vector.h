#pragma once

#include <math.h>

class Vector {
	//===============================================================
public:
	float x, y, z;

	Vector() {}

	Vector(float xx, float yy, float zz) {
		x = xx; y = yy; z = zz;
	}

	// binary operators
	Vector operator+(const Vector& c) const {
		return Vector(x + c.x, y + c.y, z + c.z);
	}

	Vector operator-(const Vector& c) const {
		return Vector(x - c.x, y - c.y, z - c.z);
	}

	Vector operator*(float f) const {
		return Vector(x * f, y * f, z * f);
	}

	Vector operator/(float f) const {
		return Vector(x / f, y / f, z / f);
	}

	Vector operator-(void) const { 
		return Vector(-x, -y, -z);
	}
	void operator+=( const Vector& v ) {  // vektor összeadás
		x += v.x; y += v.y; z += v.z; 
	}
	void operator-=( const Vector& v ) {  // vektor különbség
		x -= v.x; y -= v.y; z -= v.z; 
	}
	void operator*=( float f ) {		  // vektor és szám szorzata
		x *= f; y *= f; z *= f; 
	}

	// other methods
	float operator*(const Vector& v) const {		// DotProduct
		return x * v.x + y * v.y + z * v.z; 
	}

	Vector operator%(const Vector& v) const {		// CrossProduct
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}    
	/*Vector operator=(const Vector& v) const {		// Equation
	return Vector(v.x,v.y,v.z);
	} */

	float Len(void) const {
		return sqrt(x*x + y*y + z*z);
	}

	void Normalize() {
		float norm = Len();
		x /= norm;
		y /= norm;
		z /= norm;
	}
	Vector UnitVector( ) {				  // egy vektorral párhuzamos egységvektor
		Vector r = * this;
		r.Normalize();
		return r;
	}
};