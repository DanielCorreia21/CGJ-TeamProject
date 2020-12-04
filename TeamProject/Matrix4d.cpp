#include "Matrix4d.h"
#include <string>
#include "util.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

Matrix4d::Matrix4d()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->values[i][j] = 0;
		}
	}
}

//Constructors
Matrix4d::Matrix4d(float columnMajorArray[16])
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->values[j][i] = util::cleanFloat(columnMajorArray[i * 4 + j]);
		}
	}
}

Matrix4d::Matrix4d(float matrix[4][4])
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->values[i][j] = util::cleanFloat(matrix[i][j]);
		}
	}
}
Matrix4d::Matrix4d(Vector4d v1, Vector4d v2, Vector4d v3, Vector4d v4)
{
	this->values[0][0] = v1.getX();
	this->values[0][1] = v1.getY();
	this->values[0][2] = v1.getZ();
	this->values[0][3] = v1.getK();

	this->values[1][0] = v2.getX();
	this->values[1][1] = v2.getY();
	this->values[1][2] = v2.getZ();
	this->values[1][3] = v2.getK();

	this->values[2][0] = v3.getX();
	this->values[2][1] = v3.getY();
	this->values[2][2] = v3.getZ();
	this->values[2][3] = v3.getK();

	this->values[3][0] = v4.getX();
	this->values[3][1] = v4.getY();
	this->values[3][2] = v4.getZ();
	this->values[3][3] = v4.getK();
}
//----------------------

//General
std::string Matrix4d::toString()
{
	std::string str;
	for (int i = 0; i < 4; i++) {
		str += "[";
		for (int j = 0; j < 3; j++) {
			str += std::to_string(this->values[i][j]) + ";";
		}
		str += std::to_string(this->values[i][3]) + "]\n";
	}
	return str;
}
bool Matrix4d::isEqual(Matrix4d matrix) const
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (!util::equalFloats(this->values[i][j], matrix.values[i][j])) {
				return false;
			}
		}
	}
	return true;
}
//----------------------

//Sum
//matrix + matrix
Matrix4d Matrix4d::sumMatrix(Matrix4d matrix) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j] + matrix.values[i][j];
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator+(const Matrix4d& matrix)
{
	return this->sumMatrix(matrix);
}
//matrix += matrix 
void Matrix4d::operator+=(Matrix4d matrix)
{
	*this = *this + matrix;
}

//matrix + scalar
Matrix4d Matrix4d::sumScalar(float scalar) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j] + scalar;
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator+(float scalar) const
{
	return this->sumScalar(scalar);
}
//matrix += scalar 
void Matrix4d::operator+=(float scalar)
{
	*this = *this + scalar;
}
//scalar + matrix
Matrix4d operator+(float scalar, const Matrix4d& matrix)
{
	return matrix.sumScalar(scalar);
}
//----------------------

//Subtraction
//matrix - matrix
Matrix4d Matrix4d::subMatrix(Matrix4d matrix) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j] - matrix.values[i][j];
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator-(const Matrix4d& matrix) const
{
	return this->subMatrix(matrix);
}
//matrix -= matrix 
void Matrix4d::operator-=(Matrix4d matrix)
{
	*this = *this - matrix;
}
//matrix - scalar
Matrix4d Matrix4d::subScalar(float scalar) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j] - scalar;
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator-(float scalar) const
{
	return this->subScalar(scalar);
}
//matrix -= scalar 
void Matrix4d::operator-=(float scalar) {
	*this = *this - scalar;
}
//scalar - matrix
Matrix4d operator-(float scalar, const Matrix4d& matrix)
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = scalar - matrix.values[i][j];
		}
	}
	return Matrix4d(result);
}
//----------------------

//Multiplication
//matrix * matrix
Matrix4d Matrix4d::multMatrix(Matrix4d matrix) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result[i][j] += this->values[i][k] * matrix.values[k][j];
			}
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator*(const Matrix4d& matrix) const
{
	return this->multMatrix(matrix);
}
//matrix *= matrix 
void Matrix4d::operator*=(Matrix4d matrix)
{
	*this = *this * matrix;
}

//matrix * vector
Vector4d Matrix4d::multVector(Vector4d vector) const
{
	float result[4];
	for (int i = 0; i < 4; i++) {
		result[i] = this->values[i][0] * vector.getX() + this->values[i][1] * vector.getY()
			+ this->values[i][2] * vector.getZ() + +this->values[i][3] * vector.getK();
	}
	return Vector4d(result[0], result[1], result[2], result[3]);
}
Vector4d Matrix4d::operator*(const Vector4d& vector) const
{
	return this->multVector(vector);
}
//vector * matrix
Vector4d operator*(Vector4d vector, const Matrix4d& matrix)
{
	return matrix.multVector(vector);
}

//matrix * scalar
Matrix4d Matrix4d::multScalar(float scalar) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j] * scalar;
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator*(float scalar) const
{
	return this->multScalar(scalar);
}
//scalar * matrix
Matrix4d operator*(float scalar, const Matrix4d& matrix)
{
	return matrix.multScalar(scalar);
}
//----------------------

//Division
//matrix / scalar
Matrix4d Matrix4d::divScalar(float scalar) const
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j] / scalar;
		}
	}
	return Matrix4d(result);
}
Matrix4d Matrix4d::operator/(float scalar) const
{
	return this->divScalar(scalar);
}
//matrix /= scalar 
void Matrix4d::operator/=(float scalar) {
	*this = *this / scalar;
}
//scalar / matrix
Matrix4d operator/(float scalar, const Matrix4d& matrix)
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = scalar / matrix.values[i][j];
		}
	}
	return Matrix4d(result);
}

//Division entre matrizes não é feita por não ser possivel seguir as implementações anteriores (2D e 3D).
//----------------------

//Transpose
Matrix4d Matrix4d::transpose()
{
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[j][i];
		}
	}
	return Matrix4d(result);
}

//Convert to OpenGL
void Matrix4d::toColumnMajorArray(float out[16]) const
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out[i * 4 + j] = this->values[j][i];
		}
	}
}

Matrix4d Matrix4d::copy()
{

	float result[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = this->values[i][j];
		}
	}
	return Matrix4d(result);
}

