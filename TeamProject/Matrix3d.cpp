#include "Matrix3d.h"
#include <string>
#include "util.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

//Constructors
Matrix3d::Matrix3d(float columnMajorArray[9])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->values[j][i] = util::cleanFloat(columnMajorArray[i*3 + j]);
		}
	}
}
Matrix3d::Matrix3d(float matrix[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->values[i][j] = util::cleanFloat(matrix[i][j]);
		}
	}
}
//----------------------

//General
std::string Matrix3d::toString()
{
	std::string str;
	for (int i = 0; i < 3; i++) {
		str += "[";
		for (int j = 0; j < 2; j++) {
			str += std::to_string(this->values[i][j]) + ";";
		}
		str += std::to_string(this->values[i][2]) + "]\n";
	}
	return str;
}
bool Matrix3d::isEqual(Matrix3d matrix) const
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
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
Matrix3d Matrix3d::sumMatrix(Matrix3d matrix) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[i][j] + matrix.values[i][j];
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator+(const Matrix3d& matrix)
{
	return this->sumMatrix(matrix);
}
//matrix += matrix 
void Matrix3d::operator+=(Matrix3d matrix)
{
	*this = *this + matrix;
}
//matrix + scalar
Matrix3d Matrix3d::sumScalar(float scalar) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[i][j] + scalar;
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator+(float scalar) const
{
	return this->sumScalar(scalar);
}
//matrix += scalar 
void Matrix3d::operator+=(float scalar) {
	*this = *this + scalar;
}
//scalar + matrix
Matrix3d operator+(float scalar, const Matrix3d& matrix)
{
	return matrix.sumScalar(scalar);
}
//----------------------

//Subtraction
//matrix - matrix
Matrix3d Matrix3d::subMatrix(Matrix3d matrix) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[i][j] - matrix.values[i][j];
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator-(const Matrix3d& matrix) const
{
	return this->subMatrix(matrix);
}
//matrix -= matrix
void Matrix3d::operator-=(Matrix3d matrix)
{
	*this = *this - matrix;
}
//matrix - scalar
Matrix3d Matrix3d::subScalar(float scalar) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[i][j] - scalar;
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator-(float scalar) const
{
	return this->subScalar(scalar);
}
//matrix -= scalar 
void Matrix3d::operator-=(float scalar) {
	*this = *this - scalar;
}
//scalar - matrix
Matrix3d operator-(float scalar, const Matrix3d& matrix)
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = scalar - matrix.values[i][j];
		}
	}
	return Matrix3d(result);
}
//----------------------

//Multiplication
//matrix * matrix
Matrix3d Matrix3d::multMatrix(Matrix3d matrix) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				result[i][j] += this->values[i][k] * matrix.values[k][j];
			}
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator*(const Matrix3d& matrix) const
{
	return this->multMatrix(matrix);
}
//matrix *= matrix 
void Matrix3d::operator*=(Matrix3d matrix)
{
	*this = *this * matrix;
}
//matrix * vector
Vector3d Matrix3d::multVector(Vector3d vector) const
{
	float result[3];
	for (int i = 0; i < 3; i++) {
		result[i] = this->values[i][0] * vector.getX() + this->values[i][1] * vector.getY()
			+ this->values[i][2] * vector.getZ();
	}
	return Vector3d(result[0], result[1], result[2]);
}
Vector3d Matrix3d::operator*(const Vector3d& vector) const
{
	return this->multVector(vector);
}
//vector * matrix
Vector3d operator*(Vector3d vector, const Matrix3d& matrix)
{
	return matrix.multVector(vector);
}
//matrix * scalar
Matrix3d Matrix3d::multScalar(float scalar) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[i][j] * scalar;
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator*(float scalar) const
{
	return this->multScalar(scalar);
}
//scalar * matrix
Matrix3d operator*(float scalar, const Matrix3d& matrix)
{
	return matrix.multScalar(scalar);
}
//----------------------

//Division
//matrix / matrix
Matrix3d Matrix3d::divMatrix(Matrix3d matrix) const
{
	float determinant = matrix.determinant();
	if (determinant == 0) {
		throw "divMatrix() : Determinant is 0 !";
	}

	return *this * matrix.inverse();
}
Matrix3d Matrix3d::operator/(const Matrix3d& matrix) const
{
	return this->divMatrix(matrix);
}
//matrix /= matrix 
void Matrix3d::operator/=(Matrix3d matrix)
{
	*this = *this / matrix;
}
//matrix / vector
Vector3d Matrix3d::divVector(Vector3d vector) const
{
	float result[3];
	for (int i = 0; i < 3; i++) {
		result[i] = this->values[i][0] / vector.getX() + this->values[i][1] / vector.getY()
			+ this->values[i][2] / vector.getZ();
	}
	return Vector3d(result[0], result[1], result[2]);
}
Vector3d Matrix3d::operator/(const Vector3d& vector) const
{
	return this->divVector(vector);
}
//scalar / matrix
Vector3d operator/(Vector3d vector, const Matrix3d& matrix)
{
	float result[3];
	for (int i = 0; i < 3; i++) {
		result[i] = vector.getX() / matrix.values[i][0] + vector.getY() / matrix.values[i][1]
			+ vector.getZ() / matrix.values[i][2];
	}
	return Vector3d(result[0], result[1], result[2]);
}
//matrix / scalar
Matrix3d Matrix3d::divScalar(float scalar) const
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[i][j] / scalar;
		}
	}
	return Matrix3d(result);
}
Matrix3d Matrix3d::operator/(float scalar) const
{
	return this->divScalar(scalar);
}
//matrix /= scalar 
void Matrix3d::operator/=(float scalar) {
	*this = *this / scalar;
}
//scalar / matrix
Matrix3d operator/(float scalar, const Matrix3d& matrix)
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = scalar / matrix.values[i][j];
		}
	}
	return Matrix3d(result);
}
//----------------------

//Transpose
Matrix3d Matrix3d::transpose()
{
	float result[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = this->values[j][i];
		}
	}
	return Matrix3d(result);
}

//Determinant
float Matrix3d::determinant() const
{
	float v1 = values[0][0] * (values[1][1] * values[2][2] - values[1][2] * values[2][1]);
	float v2 = values[0][1] * (values[1][0] * values[2][2] - values[1][2] * values[2][0]);
	float v3 = values[0][2] * (values[1][0] * values[2][1] - values[1][1] * values[2][0]);
	return v1 - v2 + v3;
}

//Inverse
Matrix3d Matrix3d::inverse()
{
	float determinant = this->determinant();
	if (determinant == 0) {
		throw "Inverse() : Determinant is 0 !";
	}
	Matrix3d transpose = this->transpose();
	float adj[9] = {};
	float minor_matrices_arrays[9][4] = {
		{transpose.values[1][1],transpose.values[2][1],transpose.values[1][2],transpose.values[2][2]},
		{transpose.values[0][1],transpose.values[2][1],transpose.values[0][2],transpose.values[2][2]},
		{transpose.values[0][1],transpose.values[1][1],transpose.values[0][2],transpose.values[1][2]},
		{transpose.values[1][0],transpose.values[2][0],transpose.values[1][2],transpose.values[2][2]},
		{transpose.values[0][0],transpose.values[2][0],transpose.values[0][2],transpose.values[2][2]},
		{transpose.values[0][0],transpose.values[1][0],transpose.values[0][2],transpose.values[1][2]},
		{transpose.values[1][0],transpose.values[2][0],transpose.values[1][1],transpose.values[2][1]},
		{transpose.values[0][0],transpose.values[2][0],transpose.values[0][1],transpose.values[2][1]},
		{transpose.values[0][0],transpose.values[1][0],transpose.values[0][1],transpose.values[1][1]},
	};

	for (int i = 0; i < 9; i++) {
		adj[i] = Matrix2d(minor_matrices_arrays[i]).determinant();
	}

	for (int i = 1; i < 9; i+=2) {
		adj[i] = -adj[i];
	}
	return Matrix3d(adj).multScalar(1/determinant);

}

void Matrix3d::toColumnMajorArray(float out[9])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			out[i * 3 + j] = this->values[j][i];
		}
	}
}

//Convert to OpenGL

