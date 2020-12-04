#include "Vector4d.h"
#include "Vector3d.h"
#include <string>
#include <math.h>
#include "Matrix4d.h"
#include "util.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

Vector4d::Vector4d(float value1, float value2, float value3,float value4)
{
	this->x = value1;
	this->y = value2;
	this->z = value3;
	this->k = value4;
}

Vector4d::Vector4d() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->k = 0;
}

Vector4d::Vector4d(Vector3d vec)
{
	this->x = vec.getX();
	this->y = vec.getY();
	this->z = vec.getZ();
	this->k = 0.0;
}

//Getters and Setters
void Vector4d::setX(float value)
{
	this->x = value;
}

void Vector4d::setY(float value)
{
	this->y = value;
}

void Vector4d::setZ(float value)
{
	this->z = value;
}

void Vector4d::setK(float value)
{
	this->k = value;
}

float Vector4d::getX() const
{
	return this->x;
}

float Vector4d::getY() const
{
	return this->y;
}

float Vector4d::getZ() const
{
	return this->z;
}
float Vector4d::getK() const
{
	return this->k;
}
//-----------------------------------------------

//General 
std::string Vector4d::toString() const{
	return "(" 
		+ std::to_string(this->x) 
		+ ";" 
		+ std::to_string(this->y) 
		+ ";" 
		+ std::to_string(this->z) 
		+ ";" 
		+ std::to_string(this->k) 
		+ ")";
}
bool Vector4d::isEqual(Vector4d vec) const {

	return util::equalFloats(this->x, vec.getX())
		&& util::equalFloats(this->y, vec.getY())
		&& util::equalFloats(this->z, vec.getZ())
		&& util::equalFloats(this->k, vec.getK());
}
float Vector4d::norm() {
	return sqrt(pow(this->x, 2) + pow(this->y, 2) 
		+ pow(this->z, 2) + pow(this->k, 2));
}
Vector4d Vector4d::normalize()
{
	float normal = this->norm();
	return Vector4d(this->x / normal, this->y / normal,
		this->z / normal, this->k / normal);
}
//-----------------------------------------------

//Sum
//vector + vector
Vector4d Vector4d::sumVec(Vector4d vec) const {
	return Vector4d(this->x + vec.getX(), this->y + vec.getY(),
		this->z + vec.getZ(), this->k + vec.getK());
}
Vector4d Vector4d::operator+(const Vector4d& vec) {
	return this->sumVec(vec);
}
//vector += vector 
void Vector4d::operator+=(Vector4d  vec) {
	*this = *this + vec;
}
//vector + scalar	
Vector4d Vector4d::sumScalar(float scalar) const {
	return Vector4d(this->x + scalar, this->y + scalar,
		this->z + scalar, this->k + scalar);
}
Vector4d Vector4d::operator+(float scalar) const {
	return this->sumScalar(scalar);
}
//scalar + vector
Vector4d operator+(float scalar, const Vector4d& vec) {
	return vec.sumScalar(scalar);
}
//-----------------------------------------------

//Subtract
//vector - vector
Vector4d Vector4d::subtract(Vector4d vec) const {
	return Vector4d(this->x - vec.getX(), this->y - vec.getY(),
		this->z - vec.getZ(), this->k - vec.getK());
}
Vector4d Vector4d::operator-(const Vector4d& vec) {
	return this->subtract(vec);
}
//vector -= vector
void Vector4d::operator-=(const Vector4d& vec) {
	*this = *this - vec;
}
//vector - scalar
Vector4d Vector4d::operator-(float scalar) const {
	return Vector4d(this->x - scalar, this->y - scalar,
		this->z - scalar, this->k - scalar);
}
//scalar - vector
Vector4d operator-(float scalar, const Vector4d& vec) {
	//return vec.multScalar(-1) + scalar;
	return Vector4d(scalar - vec.getX(), scalar - vec.getY(),
		scalar - vec.getZ(), scalar - vec.getK());
}
//-----------------------------------------------

//Dot Product
// vector * vector
float Vector4d::dotProd(Vector4d vec) const {
	return (this->x * vec.getX() + this->y * vec.getY() 
		+ this->z * vec.getZ() + this->k * vec.getK());
}
float Vector4d::operator->*(const Vector4d& vec) {
	return this->dotProd(vec);
}
//-----------------------------------------------

//Multiplication
// vector * vector
Vector4d Vector4d::operator*(Vector4d vec) const {
	return Vector4d(this->x * vec.getX(), this->y * vec.getY(),
		this->z * vec.getZ(), this->k * vec.getK());
}
//vector * scalar
Vector4d Vector4d::multScalar(float scalar) const {
	return Vector4d(this->x * scalar, this->y * scalar,
		this->z * scalar, this->k * scalar);
}
Vector4d Vector4d::operator*(float scalar) const
{
	return this->multScalar(scalar);
}
//scalar * vector
Vector4d operator*(float scalar, const Vector4d& vec)
{
	return vec.multScalar(scalar);
}
//vector *= matrix
void Vector4d::operator*=(Matrix4d matrix)
{
	*this = matrix * *this;
}
//-----------------------------------------------

//Division 
//vector / vector
Vector4d Vector4d::divVector(Vector4d vec) const {
	return Vector4d(this->x / vec.getX(), this->y / vec.getY(),
		this->z / vec.getZ(), this->k / vec.getK());
}
Vector4d Vector4d::operator/(const Vector4d& vec) {
	return this->divVector(vec);
}
//vector / scalar
Vector4d Vector4d::divScalar(float scalar) const {
	return Vector4d(this->x / scalar, this->y / scalar,
		this->z / scalar, this->k / scalar);
}
Vector4d Vector4d::operator/(float scalar) const {
	return this->divScalar(scalar);
}
void Vector4d::toFloatArray(float out[4]) const
{
	out[0] = this->x;
	out[1] = this->y;
	out[2] = this->z;
	out[3] = this->k;
}
//scalar / vector
Vector4d operator/(float scalar, const Vector4d& vec) {
	return Vector4d(scalar / vec.getX(), scalar / vec.getY(),
		scalar / vec.getZ(), scalar / vec.getK());
}
//-----------------------------------------------
