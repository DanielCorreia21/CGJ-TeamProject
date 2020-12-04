#pragma once
#include <string>
#include "Vector2d.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Matrix3d;

class Matrix2d
{
private:
	float values[2][2];
public:
	//Constructors
	Matrix2d(float columnMajorArray[4]);
	Matrix2d(float matrix[2][2]);
	//----------------------

	//General
	std::string toString();
	bool isEqual(Matrix2d matrix) const;
	//----------------------

	//Sum
	//matrix + matrix
	Matrix2d sumMatrix(Matrix2d matrix) const;
	Matrix2d operator+(const Matrix2d& matrix) const;
	//matrix += matrix 
	void operator+=(Matrix2d matrix);

	//matrix + scalar
	Matrix2d sumScalar(float scalar) const;
	Matrix2d operator+(float scalar) const;
	//matrix += scalar 
	void operator+=(float scalar);
	//scalar + matrix
	friend Matrix2d operator+(float scalar, const Matrix2d& matrix);
	//----------------------

	//Subtraction
	//matrix - matrix
	Matrix2d subMatrix(Matrix2d matrix) const;
	Matrix2d operator-(const Matrix2d& matrix) const;
	//matrix -= matrix 
	void operator-=(Matrix2d matrix);

	//matrix - scalar
	Matrix2d subScalar(float scalar) const;
	Matrix2d operator-(float scalar) const;
	//matrix -= scalar 
	void operator-=(float scalar);
	//scalar - matrix
	friend Matrix2d operator-(float scalar, const Matrix2d& matrix);
	//----------------------

	//Multiplication
	//matrix * matrix
	Matrix2d multMatrix(Matrix2d matrix) const;
	Matrix2d operator*(const Matrix2d& matrix) const;
	//matrix *= matrix 
	void operator*=(Matrix2d matrix);

	//matrix * vector
	Vector2d multVector(Vector2d vector) const;
	Vector2d operator*(const Vector2d& vector) const;
	//vector * matrix
	friend Vector2d operator*(Vector2d vector, const Matrix2d& matrix);

	//matrix * scalar
	Matrix2d multScalar(float scalar) const;
	Matrix2d operator*(float scalar) const;
	//scalar * matrix
	friend Matrix2d operator*(float scalar, const Matrix2d& matrix);
	//----------------------

	//Division
	//matrix / matrix
	Matrix2d divMatrix(Matrix2d matrix) const;
	Matrix2d operator/(const Matrix2d& matrix) const;
	//matrix /= matrix 
	void operator/=(Matrix2d matrix);

	//matrix / vector
	Vector2d divVector(Vector2d vector) const;
	Vector2d operator/(const Vector2d& vector) const;
	//scalar / matrix
	friend Vector2d operator/(Vector2d vector, const Matrix2d& matrix);

	//matrix / scalar
	Matrix2d divScalar(float scalar) const;
	Matrix2d operator/(float scalar) const;
	//matrix /= scalar 
	void operator/=(float scalar);
	//scalar / matrix
	friend Matrix2d operator/(float scalar, const Matrix2d& matrix);
	//----------------------

	//Transpose
	Matrix2d transpose();

	//Determinant
	float determinant();

	//Inverse
	Matrix2d inverse();

	//Convert to OpenGL
	void toColumnMajorArray(float out[4]);
};

