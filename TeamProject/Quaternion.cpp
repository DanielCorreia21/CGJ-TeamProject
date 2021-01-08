#include "Quaternion.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

const float qThreshold = (float)1.0e-5;

//Constructors
Quaternion::Quaternion() {
	this->w = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Quaternion::Quaternion(float value1, float value2, float value3, float value4){

	this->w = value1; 
	this->x = value2;
	this->y = value3;
	this->z = value4;
}
//-----------------------------------------------

//General
Quaternion Quaternion::normalize() {

	float s = 1 / norm();
	return *this * s;
}
Quaternion Quaternion::getConjugate() {
	Quaternion qconj = { this->w, -this->x, -this->y, -this->z };
	return qconj;
}
Quaternion Quaternion::getInverse() {
	return getConjugate() * (1.0f / quadrance());
}

float Quaternion::norm() {
	return (float) sqrt(quadrance());
}

float Quaternion::quadrance()
{
	return this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z;
}

bool Quaternion::Equal(const Quaternion& q) {
	return ((float)fabs((double)this->w - q.w) < qThreshold && (float)fabs((double)this->x - q.x) < qThreshold &&
		(float)fabs((double)this->y - q.y) < qThreshold && (float)fabs((double)this->z - q.z) < qThreshold);
}

void Quaternion::clean() {
	if (fabs(this->w) < qThreshold) this->w = 0.0f;
	if (fabs(this->x) < qThreshold) this->x = 0.0f;
	if (fabs(this->y) < qThreshold) this->y = 0.0f;
	if (fabs(this->z) < qThreshold) this->z = 0.0f;
}
//-----------------------------------------------

//Convert from Quaternion
Matrix4d Quaternion::toMatrix4d() {

	Quaternion qN = this->normalize();

	float xx = qN.x * qN.x;
	float xy = qN.x * qN.y;
	float xz = qN.x * qN.z;
	float xt = qN.x * qN.w;
	float yy = qN.y * qN.y;
	float yz = qN.y * qN.z;
	float yt = qN.y * qN.w;
	float zz = qN.z * qN.z;
	float zt = qN.z * qN.w;

	return Matrix4d(new float[16]{
		1.0f - 2.0f * (yy + zz), 2.0f * (xy + zt), 2.0f * (xz - yt), 0.0f,
		2.0f * (xy - zt), 1.0f - 2.0f * (xx + zz),2.0f * (yz + xt), 0.0f,
		2.0f * (xz + yt), 2.0f * (yz - xt), 1.0f - 2.0f * (xx + yy), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f });
}

void Quaternion::toAngleAxis(float& theta, Vector4d& axis) {
	Quaternion qn = normalize();
	theta = (float) (2.0f * acos(qn.w) * (float)RADIANS_TO_DEGREES);
	float s = (float) (sqrt(1.0f - (double)qn.w * qn.w));
	if (s < qThreshold) {
		axis.setX(1.0f);
		axis.setY(0.0f);
		axis.setZ(0.0f);
		axis.setK(1.0f);
	}
	else {
		float sinv = 1 / s;
		axis.setX(qn.x * sinv);
		axis.setY(qn.y * sinv);
		axis.setZ(qn.z * sinv);
		axis.setK(1.0f);
	}
}
//-----------------------------------------------

//Convert to Quaternion
Quaternion Quaternion::fromEuler(Vector3d angles) {
	float qroll = angles.getX();
	float qpitch = angles.getY();
	float qyaw = angles.getZ();
	double cy = cos(qyaw * DEGREES_TO_RADIANS * 0.5);
	double sy = sin(qyaw * DEGREES_TO_RADIANS * 0.5);
	double cp = cos(qpitch * DEGREES_TO_RADIANS * 0.5);
	double sp = sin(qpitch * DEGREES_TO_RADIANS * 0.5);
	double cr = cos(qroll * DEGREES_TO_RADIANS * 0.5);
	double sr = sin(qroll * DEGREES_TO_RADIANS * 0.5);

	Quaternion q;
	q.w = (float) (cr * cp * cy + sr * sp * sy);
	q.x = (float) (sr * cp * cy - cr * sp * sy);
	q.y = (float) (cr * sp * cy + sr * cp * sy);
	q.z = (float) (cr * cp * sy - sr * sp * cy);

	return q;
}

Quaternion Quaternion::fromAngleAxis(float theta, Vector4d axis) {
	Vector4d axisn = axis.normalize();

	Quaternion q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.w = (float) (cos(a / 2.0f));
	float s = (float) sin(a / 2.0f);
	q.x = axisn.getX() * s;
	q.y = axisn.getY() * s;
	q.z = axisn.getZ() * s;

	q.clean();
	return q.normalize();
}

static Quaternion fromMatrix4d(Matrix4d matrix) {

	//NOT IMPLEMENTED
	return Quaternion();

	/*float m[16];
	matrix.toColumnMajorArray(m);

	// 00 01 02 03
	// 04 05 06 07
	// 08 09 10 11
	// 12 13 14 15

	float tr = m[0] + m[5] + m[10];

		if (tr > 0) {
			float S = sqrt(tr + 1.0) * 2; // S=4*qw 
			qw = 0.25 * S;
			qx = (m21 - m12) / S;
			qy = (m02 - m20) / S;
			qz = (m10 - m01) / S;
		}
		else if ((m00 > m11) & (m00 > m22)) {
			float S = sqrt(1.0 + m00 - m11 - m22) * 2; // S=4*qx 
			qw = (m21 - m12) / S;
			qx = 0.25 * S;
			qy = (m01 + m10) / S;
			qz = (m02 + m20) / S;
		}
		else if (m11 > m22) {
			float S = sqrt(1.0 + m11 - m00 - m22) * 2; // S=4*qy
			qw = (m02 - m20) / S;
			qx = (m01 + m10) / S;
			qy = 0.25 * S;
			qz = (m12 + m21) / S;
		}
		else {
			float S = sqrt(1.0 + m22 - m00 - m11) * 2; // S=4*qz
			qw = (m10 - m01) / S;
			qx = (m02 + m20) / S;
			qy = (m12 + m21) / S;
			qz = 0.25 * S;
		}*/
}

//-----------------------------------------------

//Addition
// q + q
Quaternion Quaternion::add(const Quaternion& q1) const
{
	Quaternion q;
	q.w = this->w + q1.w;
	q.x = this->x + q1.x;
	q.y = this->y + q1.y;
	q.z = this->z + q1.z;
	return q;
}
Quaternion Quaternion::operator+(const Quaternion& q) const {
	return this->add(q);
}
//-----------------------------------------------

//Multiplication
// q * q
Quaternion Quaternion::multiply(const Quaternion& q) const{
	Quaternion qq;
	qq.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;
	qq.x = this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y;
	qq.y = this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z;
	qq.z = this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x;
	return qq;
}
Quaternion Quaternion::operator*(const Quaternion& q) const {
	return this->multiply(q);
}

//q * s
Quaternion Quaternion::multiply(const float scalar) const{
	Quaternion sq;
	sq.w = scalar * this->w;
	sq.x = scalar * this->x;
	sq.y = scalar * this->y;
	sq.z = scalar * this->z;
	return sq;
}
Quaternion Quaternion::operator*(const float scalar) const {
	return this->multiply(scalar);
}
Quaternion operator*(const float scalar, const Quaternion& q) {
	return q.multiply(scalar);
}
//-----------------------------------------------

//Interpolation
Quaternion Quaternion::slerp(const Quaternion& q1, float k)
{
	float angle = (float) acosf(this->x * q1.x + this->y * q1.y + this->z * q1.z + this->w * q1.w);
	float k0 = (float) (sinf((1 - k) * angle) / sinf(angle));
	float k1 = (float) (sinf(k * angle) / sinf(angle));
	Quaternion qi = (*this * k0) + (q1 * k1);
	return qi.normalize();
}
//-----------------------------------------------