#pragma once
#include <string>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Vector2d;
class Matrix3d;

class Vector3d {
private:
	float x;
	float y;
	float z;
public:
	//Constructors
	Vector3d(float value1, float value2, float value3);
	Vector3d();
	Vector3d(Vector2d vec);
	//-----------------------------------------------

	//Setters and getters
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	float getX() const;
	float getY() const;
	float getZ() const;
	//-----------------------------------------------

	//General 
	std::string toString();
	bool isEqual(Vector3d vec) const;
	float norm(); // Magnitude of the vector
	Vector3d normalize();
	//-----------------------------------------------

	//Sum
	//vector + vector
	Vector3d sumVec( Vector3d vec) const;
	Vector3d operator+(const Vector3d& vec);
	//vector += vector 
	void operator+=(Vector3d vec);

	//vector + scalar
	Vector3d sumScalar(float scalar) const;
	Vector3d operator+(float scalar) const;
	//scalar + vector
	friend Vector3d operator+(float scalar, const Vector3d& vec);

	//-----------------------------------------------

	//Subtract 
	//vector - vector
	Vector3d subtract(Vector3d vec) const;
	Vector3d operator-(const Vector3d& vec);
	//vector -= vector
	void operator-=(const Vector3d& vec);

	//vector - scalar
	Vector3d operator-(float scalar) const;
	//scalar - vector
	friend Vector3d operator-(float scalar, const Vector3d& vec);
	//-----------------------------------------------

	//Dot Product
	//vector ->* vector
	float dotProd(Vector3d vec) const;
	float operator->*(const Vector3d&);
	//-----------------------------------------------

	//Multiplication
	//vector * vector
	Vector3d operator*(Vector3d vec) const;
	//vector * scalar
	Vector3d multScalar(float scalar) const;
	Vector3d operator*(float scalar) const;
	//scalar * vector
	friend Vector3d operator*(float scalar, const Vector3d& vec);
	//vector *= matrix
	void operator*=(Matrix3d matrix);
	//-----------------------------------------------

	//Division 
	//vector / vector
	Vector3d divVector(Vector3d vec) const;
	Vector3d operator/(const Vector3d& vec);
	//vector / scalar
	Vector3d divScalar(float scalar) const;
	Vector3d operator/(float scalar) const;
	//scalar / vector
	friend Vector3d operator/(float scalar, const Vector3d& vec);
	//-----------------------------------------------


	//Cross Product
	//vector ^ vector
	Vector3d crossProd(Vector3d vec) const;
	Vector3d operator^(const Vector3d& vec);
	//vector ^= vector
	void operator^=(const Vector3d& vec);
	//-----------------------------------------------

};

