#include "Vector2d.h"
#include <string>
#include <math.h>
#include "Vector3d.h"
#include "Matrix2d.h"
#include "util.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

Vector2d::Vector2d(float value1, float value2)
{
	this->x = value1;
	this->y = value2;
}

Vector2d::Vector2d() {
	this->x = 0;
	this->y = 0;
}

Vector2d::Vector2d(Vector3d vec)
{
	this->x = vec.getX();
	this->y = vec.getY();
}

//Setters and getters
void Vector2d::setX(float value)
{
	this->x = value;
}

void Vector2d::setY(float value)
{
	this->y = value;
}

float Vector2d::getX() const
{
	return this->x;
}

float Vector2d::getY() const
{
	return this->y;
}
//-----------------------------------------------

//General 
std::string Vector2d::toString() {
	return "(" + std::to_string(this->x) + ";" + std::to_string(this->y) + ")";
}
bool Vector2d::isEqual(Vector2d vec)  const {

	return util::equalFloats(this->x, vec.getX()) && util::equalFloats(this->y, vec.getY());
}
float Vector2d::norm() {
	return (float) sqrt(pow(this->x, 2) + pow(this->y, 2));
}
Vector2d Vector2d::normalize()
{
	float normal = this->norm();
	return Vector2d(this->x / normal, this->y / normal);
}
//-----------------------------------------------

//Sum
//vector + vector
Vector2d Vector2d::sum(Vector2d vec) const {
	return Vector2d(this->x + vec.getX(), this->y + vec.getY());
}
Vector2d Vector2d::operator+(const Vector2d& vec) {
	return this->sum(vec);
}
//vector += vector 
void Vector2d::operator+=(Vector2d  vec) {
	*this = *this + vec;
}
//vector + scalar
Vector2d Vector2d::sumScalar(float scalar) const {
	return Vector2d(this->x + scalar, this->y + scalar);
}
Vector2d Vector2d::operator+(float scalar) {
	return this->sumScalar(scalar);
}
//scalar + vector
Vector2d operator+(float scalar, const Vector2d& vec) {
	return vec.sumScalar(scalar);
}
//-----------------------------------------------

//Subtract
//vector - vector
Vector2d Vector2d::subtract(Vector2d vec) const {
	return Vector2d(this->x - vec.getX(), this->y - vec.getY());
}
Vector2d Vector2d::operator-(const Vector2d& vec) {
	return this->subtract(vec);
}
//vector -= vector
void Vector2d::operator-=(Vector2d vec) {
	*this = *this - vec;
}
//vector - scalar
Vector2d Vector2d::operator-(float scalar) {
	return Vector2d(this->x - scalar, this->y - scalar);
}
//scalar - vector
Vector2d operator-(float scalar, const Vector2d& vec) {
	return vec.multScalar(-1) + scalar;
}
//-----------------------------------------------

//Dot Product
//vector * vector
float Vector2d::dotProd(Vector2d vec)  const {
	return (this->x * vec.getX() + this->y * vec.getY());
}
float Vector2d::operator->*(const Vector2d& vec) {
	return this->dotProd(vec);
}
//-----------------------------------------------

//Multiplication
//vector * vector
Vector2d Vector2d::operator*(Vector2d vec)  const {
	return Vector2d(this->x * vec.getX(), this->y * vec.getY());
}
//vector * scalar
Vector2d Vector2d::multScalar(float scalar)  const {
	return Vector2d(this->x * scalar, this->y * scalar);
}
Vector2d Vector2d::operator*(float scalar) const
{
	return this->multScalar(scalar);
}
//scalar * vector
Vector2d operator*(float scalar, const Vector2d& vec) {
	return vec.multScalar(scalar);
}
//vector *= matrix
void Vector2d::operator*=(Matrix2d matrix)
{
	*this = matrix * *this;
}
//-----------------------------------------------

//Division 
//vector / vector
Vector2d Vector2d::divVector(Vector2d vec)  const {
	return Vector2d(this->x / vec.getX(), this->y / vec.getY());
}
Vector2d Vector2d::operator/(const Vector2d& vec) {
	return this->divVector(vec);
}
//vector / scalar
Vector2d Vector2d::divScalar(float scalar)  const {
	return Vector2d(this->x / scalar, this->y / scalar);
}
Vector2d Vector2d::operator/(float scalar)  const {
	return this->divScalar(scalar);
}
//scalar / vector
Vector2d operator/(float scalar, const Vector2d& vec) {
	return Vector2d(scalar / vec.getX(), scalar / vec.getY());
}
//-----------------------------------------------




