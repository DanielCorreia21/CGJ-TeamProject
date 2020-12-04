#pragma once
#include <string>
#include "Matrix2d.h"
#include "Vector3d.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Matrix3d
{
private:
	float values[3][3];
public:
	//Constructors
	Matrix3d(float columnMajorArray[9]);
	Matrix3d(float matrix[3][3]);
	//----------------------

	//General
	std::string toString();
	bool isEqual(Matrix3d matrix) const;
	//----------------------

	//Sum
	//matrix + matrix
	Matrix3d sumMatrix(Matrix3d matrix) const;
	Matrix3d operator+(const Matrix3d& matrix);
	//matrix += matrix 
	void operator+=(Matrix3d matrix);

	//matrix + scalar
	Matrix3d sumScalar(float scalar) const;
	Matrix3d operator+(float scalar) const;
	//matrix += scalar 
	void operator+=(float scalar);
	//scalar + matrix
	friend Matrix3d operator+(float scalar, const Matrix3d& matrix);
	//----------------------

	//Subtraction
	//matrix - matrix
	Matrix3d subMatrix(Matrix3d matrix) const;
	Matrix3d operator-(const Matrix3d& matrix) const;
	//matrix -= matrix 
	void operator-=(Matrix3d matrix);

	//matrix - scalar
	Matrix3d subScalar(float scalar) const;
	Matrix3d operator-(float scalar) const;
	//matrix -= scalar 
	void operator-=(float scalar);
	//scalar - matrix
	friend Matrix3d operator-(float scalar, const Matrix3d& matrix);
	//----------------------

	//Multiplication
	//matrix * matrix
	Matrix3d multMatrix(Matrix3d matrix) const;
	Matrix3d operator*(const Matrix3d& matrix) const;
	//matrix *= matrix 
	void operator*=(Matrix3d matrix);

	//matrix * vector
	Vector3d multVector(Vector3d vector) const;
	Vector3d operator*(const Vector3d& vector) const;
	//vector * matrix
	friend Vector3d operator*(Vector3d vector, const Matrix3d& matrix);
	
	//matrix * scalar
	Matrix3d multScalar(float scalar) const;
	Matrix3d operator*(float scalar) const;
	//scalar * matrix
	friend Matrix3d operator*(float scalar, const Matrix3d& matrix);
	//----------------------

	//Division
	//matrix / matrix
	Matrix3d divMatrix(Matrix3d matrix) const;
	Matrix3d operator/(const Matrix3d& matrix) const;
	//matrix /= matrix 
	void operator/=(Matrix3d matrix);

	//matrix / vector
	Vector3d divVector(Vector3d vector) const;
	Vector3d operator/(const Vector3d& vector) const;
	//scalar / matrix
	friend Vector3d operator/(Vector3d vector, const Matrix3d& matrix);

	//matrix / scalar
	Matrix3d divScalar(float scalar) const;
	Matrix3d operator/(float scalar) const;
	//matrix /= scalar 
	void operator/=(float scalar);
	//scalar / matrix
	friend Matrix3d operator/(float scalar, const Matrix3d& matrix);
	//----------------------

	//Transpose
	Matrix3d transpose();

	//Determinant
	float determinant() const;

	//Inverse
	Matrix3d inverse();

	//Convert to OpenGL
	void toColumnMajorArray(float out[9]);
};

