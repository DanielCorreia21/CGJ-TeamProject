#include "MatrixFactory.h"
#include <math.h>

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

const float PI = 3.141592f;

Matrix2d MatrixFactory::identityMatrix2d()
{
    float array[2][2] = {
        {1,0},
        {0,1}
    };
    return Matrix2d(array);
}

Matrix3d MatrixFactory::identityMatrix3d()
{
    float array[3][3] = {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };
    return Matrix3d(array);
}

Matrix4d MatrixFactory::identityMatrix4d()
{
    float array[4][4] = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
    };
    return Matrix4d(array);
}

Matrix3d MatrixFactory::dualMatrix(Vector3d vector) {
    float result[3][3] = {
        {0, - vector.getZ(), vector.getY()},
        {vector.getZ(), 0, vector.getX()},
        {- vector.getY(), vector.getX(), 0}
    };
    return Matrix3d(result);
}

Matrix4d MatrixFactory::scalingMatrix(Vector3d vector) {
    float result[4][4] = {
        {vector.getX(), 0, 0, 0},
        {0, vector.getY(), 0, 0},
        {0, 0, vector.getZ(), 0},
        {0, 0, 0, 1},
    };
    return Matrix4d(result);
}

Matrix4d MatrixFactory::scalingMatrix(float value)
{
    float result[4][4] = {
       {value, 0, 0, 0},
       {0, value, 0, 0},
       {0, 0, value, 0},
       {0, 0, 0, 1},
    };
    return Matrix4d(result);
}

Matrix4d MatrixFactory::translationMatrix(Vector3d vector) {
    float result[4][4] = {
        {1,0,0,vector.getX()},
        {0,1,0,vector.getY()},
        {0,0,1,vector.getZ()},
        {0,0,0,1}
    };
    return Matrix4d(result);
}

Matrix4d MatrixFactory::rotateXMatrix(float angle)
{
    float radianAngle = angle * PI / 180;
    float result[4][4] = {
        {1, 0, 0, 0},
        {0, (float) cos(radianAngle), (float)-sin(radianAngle), 0},
        {0, (float)sin(radianAngle), (float) cos(radianAngle), 0},
        {0, 0, 0, 1},
    };
    return Matrix4d(result);
}

Matrix4d MatrixFactory::rotateYMatrix(float angle)
{
    float radianAngle = angle * PI / 180;

    float result[4][4] = {
        {(float) cos(radianAngle), 0,(float)sin(radianAngle), 0},
        {0, 1, 0, 0},
        {(float) -sin(radianAngle), 0,(float)cos(radianAngle), 0},
        {0, 0, 0, 1},
    };
    return Matrix4d(result);
}

Matrix4d MatrixFactory::rotateZMatrix(float angle)
{
    float radianAngle = angle * PI / 180;

    float result[4][4] = {
        {(float) cos(radianAngle),(float) -sin(radianAngle), 0, 0},
        {(float) sin(radianAngle),(float) cos(radianAngle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };
    return Matrix4d(result);
}
