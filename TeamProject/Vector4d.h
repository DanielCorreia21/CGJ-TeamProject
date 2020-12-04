#pragma once
#include <string>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Vector3d;
class Matrix4d;

class Vector4d
{
private:
	float x;
	float y;
	float z;
	float k;
public:
	//Constructors
	Vector4d(float value1, float value2, float value3,float value4);
	Vector4d();
	Vector4d(Vector3d vec);
	//-----------------------------------------------

	//Setters and getters
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	void setK(float value);
	float getX() const;
	float getY() const;
	float getZ() const;
	float getK() const;
	//-----------------------------------------------

	//General 
	std::string toString() const;
	bool isEqual(Vector4d vec) const;
	float norm(); // Magnitude of the vector
	Vector4d normalize();
	//-----------------------------------------------

	//Sum
	//vector + vector
	Vector4d sumVec(Vector4d vec) const;
	Vector4d operator+(const Vector4d& vec);
	//vector += vector 
	void operator+=(Vector4d vec);

	//vector + scalar
	Vector4d sumScalar(float scalar) const;
	Vector4d operator+(float scalar) const;
	//scalar + vector
	friend Vector4d operator+(float scalar, const Vector4d& vec);

	//-----------------------------------------------

	//Subtract 
	//vector - vector
	Vector4d subtract(Vector4d vec) const;
	Vector4d operator-(const Vector4d& vec);
	//vector -= vector
	void operator-=(const Vector4d& vec);

	//vector - scalar
	Vector4d operator-(float scalar) const;
	//scalar - vector
	friend Vector4d operator-(float scalar, const Vector4d& vec);
	//-----------------------------------------------

	//Dot Product
	//vector ->* vector
	float dotProd(Vector4d vec) const;
	float operator->*(const Vector4d&);
	//-----------------------------------------------

	//Multiplication
	//vector * vector
	Vector4d operator*(Vector4d vec) const;
	//vector * scalar
	Vector4d multScalar(float scalar) const;
	Vector4d operator*(float scalar) const;
	//scalar * vector
	friend Vector4d operator*(float scalar, const Vector4d& vec);
	//vector *= matrix
	void operator*=(Matrix4d matrix);
	//-----------------------------------------------

	//Division 
	//vector / vector
	Vector4d divVector(Vector4d vec) const;
	Vector4d operator/(const Vector4d& vec);
	//vector / scalar
	Vector4d divScalar(float scalar) const;
	Vector4d operator/(float scalar) const;
	//scalar / vector
	friend Vector4d operator/(float scalar, const Vector4d& vec);
	//-----------------------------------------------

	void toFloatArray(float out[4]) const;
};

