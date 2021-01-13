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
	CameraType currentType;
	RotationMode currentRotation;
	Vector3d translationVector;
	Matrix4d initialView;
	int menu;

	Camera(Vector3d eye, Vector3d front, Vector3d up);
	Matrix4d getOrthoProj(double left, double right, double bottom, double top, double near, double far);
	Matrix4d getPerspectiveProj(float fov, float aspect, float near, float far);
	Matrix4d getViewMatrix();
	Matrix4d getProjectionMatrix();
	void applyRotation(float angleAroundX, float angleAroundY, float angleAroundZ);
	void updateCameraPos(int key, int action);
	void initBuffer();
	void destroyBuffer();
	void draw();
	void look(float xpos, float ypos, bool pressed);
	void changeProjectionType();
	void changeRotationType();
	void updateCamera();
	Vector3d getEulerAngles();
	void setEulerAngles(Vector3d angles);
private:
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
	bool keys[4] = { false,false,false,false };
	bool was_pressed = false;
	bool firstMouse = true;
	float lastX = 0;
	float lastY = 0;
	Vector3d up, cameraOrientation;
	Matrix4d orthoMatrix, perspectiveMatrix;
	GLuint VboId[1];
	float CAMERA_SPEED = 0.05f;
	Matrix4d getRotationMatrix();
};

