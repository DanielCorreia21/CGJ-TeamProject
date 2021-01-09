#include "Vector3d.h"
#include <string>
#include <math.h> 
#include "Vector2d.h"
#include "Matrix3d.h"
#include "util.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

Vector3d::Vector3d(float value1, float value2, float value3)
{
	this->x = value1;
	this->y = value2;
	this->z = value3;
}

Vector3d::Vector3d() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3d::Vector3d(Vector2d vec)
{
	this->x = vec.getX();
	this->y = vec.getY();
	this->z = 0.0;
}

//Getters and Setters
void Vector3d::setX(float value)
{
	this->x = value;
}

void Vector3d::setY(float value)
{
	this->y = value;
}

void Vector3d::setZ(float value)
{
	this->z = value;
}

float Vector3d::getX() const
{
	return this->x;
}

float Vector3d::getY() const
{
	return this->y;
}

float Vector3d::getZ() const
{
	return this->z;
}
//-----------------------------------------------

//General 
std::string Vector3d::toString() {
	return "(" + std::to_string(this-> x) + ";" + std::to_string(this-> y) + ";" + std::to_string(this-> z) + ")";
}
bool Vector3d::isEqual(Vector3d vec) const{
	
	return util::equalFloats(this->x, vec.getX()) && util::equalFloats(this->y, vec.getY()) && util::equalFloats(this->z, vec.getZ());
}
float Vector3d::norm() {
	return (float) sqrt(pow(this-> x,2) + pow(this-> y, 2) + pow(this-> z, 2));
}
Vector3d Vector3d::normalize()
{
	float normal = this->norm();
	return Vector3d(this->x/normal, this->y/normal, this->z/normal);
}
//-----------------------------------------------

//Sum
//vector + vector
Vector3d Vector3d::sumVec(Vector3d vec) const {
	return Vector3d(this -> x + vec.getX(), this -> y + vec.getY(), this -> z + vec.getZ());
}
Vector3d Vector3d::operator+(const Vector3d& vec) {
	return this->sumVec(vec);
}
//vector += vector 
void Vector3d::operator+=(Vector3d  vec) {
	*this = *this + vec;
}
//vector + scalar	
Vector3d Vector3d::sumScalar(float scalar) const {
	return Vector3d(this->x + scalar, this->y + scalar, this->z + scalar);
}
Vector3d Vector3d::operator+(float scalar) const {
	return this->sumScalar(scalar);
}
//scalar + vector
Vector3d operator+(float scalar, const Vector3d& vec) {
	return vec.sumScalar(scalar);
}
//-----------------------------------------------

//Subtract
//vector - vector
Vector3d Vector3d::subtract(Vector3d vec) const{
	return Vector3d(this->x - vec.getX(), this->y - vec.getY(), this->z - vec.getZ());
}
Vector3d Vector3d::operator-(const Vector3d& vec) {
	return this->subtract(vec);
}
//vector -= vector
void Vector3d::operator-=(const Vector3d& vec) {
	*this = *this - vec;
}
//vector - scalar
Vector3d Vector3d::operator-(float scalar) const {
	return Vector3d(this->x - scalar, this->y - scalar, this->z - scalar);
}
//scalar - vector
Vector3d operator-(float scalar, const Vector3d& vec) {
	//return vec.multScalar(-1) + scalar;
	return Vector3d(scalar - vec.getX(), scalar - vec.getY(), scalar - vec.getZ());
}
//-----------------------------------------------

//Dot Product
// vector * vector
float Vector3d::dotProd(Vector3d vec) const{
	return (this -> x * vec.getX() + this -> y * vec.getY() + this -> z * vec.getZ());
}
float Vector3d::operator->*(const Vector3d& vec) {
	return this->dotProd(vec);
}
//-----------------------------------------------

//Multiplication
// vector * vector
Vector3d Vector3d::operator*(Vector3d vec) const{
	return Vector3d(this->x * vec.getX(), this->y * vec.getY(),this->z * vec.getZ());
}
//vector * scalar
Vector3d Vector3d::multScalar(float scalar) const {
	return Vector3d(this->x * scalar, this->y * scalar, this->z * scalar);
}
Vector3d Vector3d::operator*(float scalar) const
{
	return this->multScalar(scalar);
}
//scalar * vector
Vector3d operator*(float scalar, const Vector3d& vec)
{
	return vec.multScalar(scalar);
}
//vector *= matrix
void Vector3d::operator*=(Matrix3d matrix)
{
	*this = matrix * *this;
}
//-----------------------------------------------

//Division 
//vector / vector
Vector3d Vector3d::divVector(Vector3d vec) const{
	return Vector3d(this->x / vec.getX(), this->y / vec.getY(), this->z / vec.getZ());
}
Vector3d Vector3d::operator/(const Vector3d& vec) {
	return this->divVector(vec);
}
//vector / scalar
Vector3d Vector3d::divScalar(float scalar) const{
	return Vector3d(this->x / scalar, this->y / scalar, this->z / scalar);
}
Vector3d Vector3d::operator/(float scalar) const{
	return this->divScalar(scalar);
}
//scalar / vector
Vector3d operator/(float scalar, const Vector3d& vec) {
	return Vector3d(scalar / vec.getX(), scalar / vec.getY(), scalar / vec.getZ());
}
//-----------------------------------------------

//Cross Product
//vector ^ vector
Vector3d Vector3d::crossProd(Vector3d vec) const{
	float x = this->y * vec.getZ() - this->z * vec.getY();
	float y = this->z * vec.getX() - this->x * vec.getZ();
	float z = this->x * vec.getY() - this->y * vec.getX();
	return Vector3d(x,y,z);
}
Vector3d Vector3d::operator^(const Vector3d& vec) {
	return this->crossProd(vec);
}
//vector ^= vector
void Vector3d::operator^=(const Vector3d& vec) {
	*this = *this ^ vec;
}
//-----------------------------------------------


