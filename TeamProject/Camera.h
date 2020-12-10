#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Matrix4d.h"
#include "MatrixFactory.h"
#include "Vector4d.h"
#include "Quaternion.h"
#include "ShaderProgram.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

enum Camera_Type {
	ORTHOGONAL,
	PERSPECTIVE
};

enum Rotation_Mode {
	EULER,
	QUATERNION
};

class Camera
{
private:
	Vector3d up, cameraOrientation;
	Vector3d translationVector;
	Camera_Type currentType;
	Rotation_Mode currentRotation;
	Matrix4d orthoMatrix, persectiveMatrix;
	GLuint VboId[1];
	float CAMERA_SPEED = 0.05;
	Matrix4d getRotationMatrix();
public:
	Camera(Vector3d eye, Vector3d front, Vector3d up);
	Matrix4d getViewMatrix();
	Matrix4d getProjectionMatrix();
	void applyRotation(float angleAroundX, float angleAroundY, float angleAroundZ);
	void updateCameraPos(int key, int action);
	void initBuffer(ShaderProgram shaders);
	void destroyBuffer();
	void draw();
	void look(float xpos, float ypos, bool pressed);
	void changeProjectionType();
	void changeRotationType();
	void updateCamera();
};

