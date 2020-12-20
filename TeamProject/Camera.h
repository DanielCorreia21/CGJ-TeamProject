#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Matrix4d.h"
#include "MatrixFactory.h"
#include "Vector4d.h"
#include "Quaternion.h"
#include "ShaderProgram.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class Camera
{
public:
	enum class CameraType {
		ORTHOGONAL,
		PERSPECTIVE
	};

	enum class RotationMode {
		EULER,
		QUATERNION
	};

	Camera(Vector3d eye, Vector3d front, Vector3d up);
	Matrix4d getViewMatrix();
	Matrix4d getProjectionMatrix();
	void applyRotation(float angleAroundX, float angleAroundY, float angleAroundZ);
	void updateCameraPos(int key, int action);
	void initBuffer(ShaderProgram* shaders);
	void destroyBuffer();
	void draw();
	void look(float xpos, float ypos, bool pressed);
	void changeProjectionType();
	void changeRotationType();
	void updateCamera();
private:
	Vector3d up, cameraOrientation;
	Vector3d translationVector;
	CameraType currentType;
	RotationMode currentRotation;
	Matrix4d orthoMatrix, persectiveMatrix;
	GLuint VboId[1];
	float CAMERA_SPEED = 0.05f;
	Matrix4d getRotationMatrix();
};

