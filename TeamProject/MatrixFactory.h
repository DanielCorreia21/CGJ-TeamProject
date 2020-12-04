#pragma once
#include "Matrix2d.h"
#include "Matrix3d.h"
#include "Vector3d.h"
#include "Matrix4d.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class MatrixFactory
{
public:
	static Matrix2d identityMatrix2d();
	static Matrix3d identityMatrix3d();
	static Matrix4d identityMatrix4d();
	static Matrix3d dualMatrix(Vector3d vector);
	static Matrix4d scalingMatrix(Vector3d vector);
	static Matrix4d scalingMatrix(float value);
	static Matrix4d translationMatrix(Vector3d vector);
	static Matrix4d rotateXMatrix(float angle);
	static Matrix4d rotateYMatrix(float angle);
	static Matrix4d rotateZMatrix(float angle);
};

