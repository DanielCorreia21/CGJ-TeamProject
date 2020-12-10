#pragma once
#include "Matrix4d.h"
#include "Vector4d.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Quaternion
{
public:
	float w;
	float x;
	float y;
	float z;
	//Constructors
	Quaternion();
	Quaternion(float value1, float value2, float value3, float value4);
	//-----------------------------------------------

	//General
	Quaternion normalize();
	Quaternion getConjugate();
	Quaternion getInverse();
	float norm();
	float quadrance();
	bool Equal(const Quaternion& q);

	void clean();
	//-----------------------------------------------

	//Convert from Quaternion
	Matrix4d toMatrix4d();
	void toAngleAxis(float& theta, Vector4d& axis);
	//-----------------------------------------------

	//Convert to Quaternion
	static Quaternion fromEuler(Vector3d angles);
	static Quaternion fromAngleAxis(float theta, Vector4d axis);
	static Quaternion fromMatrix4d(Matrix4d matrix);
	//-----------------------------------------------

	//Multiplication
	// q + q
	Quaternion add(const Quaternion& q1) const;
	Quaternion operator+(const Quaternion& q) const;
	//-----------------------------------------------

	//Multiplication
	// q * q
	// Multiplying q1 with q2 applies the rotation q2 to q1 Quaternion 
	Quaternion multiply(const Quaternion& q) const;
	Quaternion operator*(const Quaternion& q) const;

	Quaternion multiply(const float scalar) const;
	Quaternion operator*(const float scalar) const;
	friend Quaternion operator*(const float scalar, const Quaternion& q);
	//-----------------------------------------------

	//Interpolation
	Quaternion slerp(const Quaternion& q1, float k);
	//-----------------------------------------------
};

