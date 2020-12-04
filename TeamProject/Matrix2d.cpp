#include "Matrix2d.h"
#include "Matrix3d.h"
#include "Vector2d.h"
#include "util.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

//Constructorsdwa
Matrix2d::Matrix2d(float columnMajorArray[4])
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			this->values[j][i] = util::cleanFloat(columnMajorArray[i * 2 + j]);
		}
	}
}
Matrix2d::Matrix2d(float matrix[2][2])
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			this->values[i][j] = util::cleanFloat(matrix[i][j]);
		}
	}
}
//----------------------

//General
std::string Matrix2d::toString()
{
	std::string str;
	for (int i = 0; i < 2; i++) {
		str += "[";
		str += std::to_string(this->values[i][0]) + ";";
		str += std::to_string(this->values[i][1]) + "]\n";
	}
	return str;
}
bool Matrix2d::isEqual(Matrix2d matrix) const
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
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
Matrix2d Matrix2d::sumMatrix(Matrix2d matrix) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] + matrix.values[i][j];
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator+(const Matrix2d& matrix) const
{
	return this->sumMatrix(matrix);
}
//matrix += matrix
void Matrix2d::operator+=(Matrix2d matrix)
{
	*this = *this + matrix;
}
//matrix + scalar
Matrix2d Matrix2d::sumScalar(float scalar) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] + scalar;
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator+(float scalar) const
{
	return this->sumScalar(scalar);
}
//matrix += scalar 
void Matrix2d::operator+=(float scalar) {
	*this = *this + scalar;
}
//scalar + matrix
Matrix2d operator+(float scalar, const Matrix2d& matrix)
{
	return matrix.sumScalar(scalar);
}
//----------------------

//Subtraction
//matrix - matrix
Matrix2d Matrix2d::subMatrix(Matrix2d matrix) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] - matrix.values[i][j];
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator-(const Matrix2d& matrix) const
{
	return this->subMatrix(matrix);
}
//matrix -= matrix
void Matrix2d::operator-=(Matrix2d matrix)
{
	*this = *this - matrix;
}
//matrix - scalar
Matrix2d Matrix2d::subScalar(float scalar) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] - scalar;
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator-(float scalar) const
{
	return this->subScalar(scalar);
}
//matrix -= scalar 
void Matrix2d::operator-=(float scalar) {
	*this = *this - scalar;
}
//scalar - matrix
Matrix2d operator-(float scalar, const Matrix2d& matrix)
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = scalar - matrix.values[i][j];
		}
	}
	return Matrix2d(result);
}
//----------------------

//Multiplication
//matrix * matrix
Matrix2d Matrix2d::multMatrix(Matrix2d matrix) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] * matrix.values[i][j];
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator*(const Matrix2d& matrix) const
{
	return this->multMatrix(matrix);
}
//matrix *= matrix
void Matrix2d::operator*=(Matrix2d matrix)
{
	*this = *this * matrix;
}
//matrix * vector
Vector2d Matrix2d::multVector(Vector2d vector) const
{
	float result[2];
	for (int i = 0; i < 2; i++) {
		result[i] = this->values[i][0] * vector.getX() + this->values[i][1] * vector.getY();
	}
	return Vector2d(result[0], result[1]);
}
Vector2d Matrix2d::operator*(const Vector2d& vector) const
{
	return this->multVector(vector);
}
//vector * matrix
Vector2d operator*(Vector2d vector, const Matrix2d& matrix)
{
	return matrix.multVector(vector);
}
//matrix * scalar
Matrix2d Matrix2d::multScalar(float scalar) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] * scalar;
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator*(float scalar) const
{
	return this->multScalar(scalar);
}
//scalar * matrix
Matrix2d operator*(float scalar, const Matrix2d& matrix)
{
	return matrix.multScalar(scalar);
}
//----------------------

//Division
//matrix / matrix
Matrix2d Matrix2d::divMatrix(Matrix2d matrix) const
{
	float determinant = matrix.determinant();
	if (determinant == 0) {
		throw "divMatrix() : Determinant is 0 !";
	}

	return *this * matrix.inverse();
}
Matrix2d Matrix2d::operator/(const Matrix2d& matrix) const
{
	return this->divMatrix(matrix);
}
//matrix /= matrix 
void Matrix2d::operator/=(Matrix2d matrix)
{
	*this = *this / matrix;
}

//matrix / vector
Vector2d Matrix2d::divVector(Vector2d vector) const
{
	float result[2];
	for (int i = 0; i < 2; i++) {
		result[i] = this->values[i][0] / vector.getX() + this->values[i][1] / vector.getY();
	}
	return Vector2d(result[0], result[1]);
}
Vector2d Matrix2d::operator/(const Vector2d& vector) const
{
	return this->divVector(vector);
}
//scalar / matrix
Vector2d operator/(Vector2d vector, const Matrix2d& matrix)
{
	float result[2];
	for (int i = 0; i < 2; i++) {
		result[i] = vector.getX() / matrix.values[i][0] + vector.getY() / matrix.values[i][1];
	}
	return Vector2d(result[0], result[1]);
}

//matrix / scalar
Matrix2d Matrix2d::divScalar(float scalar) const
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[i][j] / scalar;
		}
	}
	return Matrix2d(result);
}
Matrix2d Matrix2d::operator/(float scalar) const
{
	return this->divScalar(scalar);
}
//matrix /= scalar 
void Matrix2d::operator/=(float scalar) {
	*this = *this / scalar;
}
//scalar / matrix
Matrix2d operator/(float scalar, const Matrix2d& matrix)
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = scalar / matrix.values[i][j];
		}
	}
	return Matrix2d(result);
}
//----------------------

//Transpose
Matrix2d Matrix2d::transpose()
{
	float result[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = this->values[j][i];
		}
	}
	return Matrix2d(result);
}

// Determinant
float Matrix2d::determinant()
{
	//return c1[0]*c2[1] - c2[0]*c1[1];
	return values[0][0] * values[1][1] - values[0][1] * values[1][0];
}

//Inverse
Matrix2d Matrix2d::inverse()
{
	float determinant = this->determinant();
	if (determinant == 0) {
		throw "inverse() : Determinant is 0 !";
	}
	float result[2][2] = {
		{this->values[1][1], -1 * this->values[0][1]},
		{ -1 * this->values[1][0],this->values[0][0]}
	};
	return Matrix2d(result).multScalar(1 / determinant);

}

//Convert to OpenGL
void Matrix2d::toColumnMajorArray(float out[4])
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			out[i * 2 + j] = this->values[j][i];
		}
	}
}
