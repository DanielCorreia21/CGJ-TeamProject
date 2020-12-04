#pragma once

#include <string>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class Vector3d;
class Matrix2d;

class Vector2d
{
private:
	float x;
	float y;
public:
	//Constructors
	Vector2d(float value1, float value2);
	Vector2d();
	Vector2d(Vector3d vec);
	//-----------------------------------------------

	//Setters and getters
	void setX(float value);
	void setY(float value);
	float getX() const;
	float getY() const;
	//-----------------------------------------------

	//General 
	std::string toString();
	bool isEqual(Vector2d vec) const;
	float norm();
	Vector2d normalize();
	//-----------------------------------------------

	//Sum
	//vector + vector
	Vector2d sum(Vector2d vec) const;
	Vector2d operator+(const Vector2d& vec);
	//vector += vector 
	void operator+=(Vector2d vec);
	//vector + scalar
	Vector2d sumScalar(float scalar) const;
	Vector2d operator+(float scalar);
	//scalar + vector
	friend Vector2d operator+(float scalar, const Vector2d& vec);
	//-----------------------------------------------

	//Subtract 
	//vector - vector
	Vector2d subtract(Vector2d vec) const;
	Vector2d operator-(const Vector2d& vec);
	//vector -= vector
	void operator-=(Vector2d vec);

	//vector - scalar
	Vector2d operator-(float scalar);
	//scalar - vector
	friend Vector2d operator-(float scalar, const Vector2d& vec);
	//-----------------------------------------------

	//Dot Product
	//vector * vector
	float dotProd(Vector2d vec) const;
	float operator->*(const Vector2d& vec);
	//-----------------------------------------------

	//Multiplication
	//vector * vector
	Vector2d operator*(Vector2d vec) const;
	//vector * scalar
	Vector2d multScalar(float scalar) const;
	Vector2d operator*(float scalar) const;
	//scalar * vector
	friend Vector2d operator*(float scalar, const Vector2d& vec);
	//vector *= matrix
	void operator*=(Matrix2d matrix);
	//-----------------------------------------------

	//Division 
	//vector / vector
	Vector2d divVector(Vector2d vec) const;
	Vector2d operator/(const Vector2d& vec) ;
	//vector / scalar
	Vector2d divScalar(float scalar) const;
	Vector2d operator/(float scalar) const;
	//scalar / vector
	friend Vector2d operator/(float scalar, const Vector2d& vec);
	//-----------------------------------------------


};
