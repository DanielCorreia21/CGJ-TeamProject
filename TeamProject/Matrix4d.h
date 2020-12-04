#pragma once
#include <string>
#include "Matrix3d.h"
#include "Vector4d.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Matrix4d
{
private:
	float values[4][4];
public:
	//Constructors
	Matrix4d();
	Matrix4d(float columnMajorArray[16]);
	Matrix4d(float matrix[4][4]);
	Matrix4d(Vector4d v1, Vector4d v2, Vector4d v3, Vector4d v4);
	//----------------------

	//General
	std::string toString();
	bool isEqual(Matrix4d matrix) const;
	//----------------------

	//Sum
	//matrix + matrix
	Matrix4d sumMatrix(Matrix4d matrix) const;
	Matrix4d operator+(const Matrix4d& matrix);
	//matrix += matrix 
	void operator+=(Matrix4d matrix);

	//matrix + scalar
	Matrix4d sumScalar(float scalar) const;
	Matrix4d operator+(float scalar) const;
	//matrix += scalar 
	void operator+=(float scalar);
	//scalar + matrix
	friend Matrix4d operator+(float scalar, const Matrix4d& matrix);
	//----------------------

	//Subtraction
	//matrix - matrix
	Matrix4d subMatrix(Matrix4d matrix) const;
	Matrix4d operator-(const Matrix4d& matrix) const;
	//matrix -= matrix 
	void operator-=(Matrix4d matrix);

	//matrix - scalar
	Matrix4d subScalar(float scalar) const;
	Matrix4d operator-(float scalar) const;
	//matrix -= scalar 
	void operator-=(float scalar);
	//scalar - matrix
	friend Matrix4d operator-(float scalar, const Matrix4d& matrix);
	//----------------------

	//Multiplication
	//matrix * matrix
	Matrix4d multMatrix(Matrix4d matrix) const;
	Matrix4d operator*(const Matrix4d& matrix) const;
	//matrix *= matrix 
	void operator*=(Matrix4d matrix);

	//matrix * vector
	Vector4d multVector(Vector4d vector) const;
	Vector4d operator*(const Vector4d& vector) const;
	//vetor * matrix
	friend Vector4d operator*(Vector4d vector, const Matrix4d& matrix);

	//matrix * scalar
	Matrix4d multScalar(float scalar) const;
	Matrix4d operator*(float scalar) const;
	//scalar * matrix
	friend Matrix4d operator*(float scalar, const Matrix4d& matrix);
	//----------------------

	//Division
	//matrix / scalar
	Matrix4d divScalar(float scalar) const;
	Matrix4d operator/(float scalar) const;
	//matrix /= scalar 
	void operator/=(float scalar);
	//scalar / matrix
	friend Matrix4d operator/(float scalar, const Matrix4d& matrix);
	//Division entre matrizes não é feita por não ser possivel seguir as implementações anteriores (2D e 3D).
	//----------------------

	//Transpose
	Matrix4d transpose();

	//Convert to OpenGL
	void toColumnMajorArray(float out[16]) const;

	Matrix4d copy();
};

