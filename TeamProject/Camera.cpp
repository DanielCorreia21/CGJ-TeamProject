#include "Camera.h"
#include "GLStatics.h"
#include <iostream>

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/
const double PI = 3.141592;

Matrix4d Camera::getOrthoProj(double left, double right, double bottom, double top, double near, double far) {
	float result_array[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result_array[i][j] = 0;
		}
	}
	result_array[0][0] = (float) (2 / (right - left));
	result_array[1][1] = (float) (2 / (top - bottom));
	result_array[2][2] = (float) (2 / (near - far));
	result_array[0][3] = (float) ((left + right) / (left - right));
	result_array[1][3] = (float) ((bottom + top) / (bottom - top));
	result_array[2][3] = (float) ((near + far) / (near - far));
	result_array[3][3] = (float) 1;

	return Matrix4d(result_array);
}

Matrix4d Camera::getPerspectiveProj(float fov, float aspect, float near, float far) {

	float theta = fov / 2;
	float d = (float) (1 / (tan(theta * PI / 180)));
	//float aspect = aspect;
	float A = (near + far) / (near - far);
	float B = (2 * near * far) / (near - far);

	float P[4][4] = {
		{d / aspect, 0 ,  0 , 0},
		{0       , d ,  0 , 0},
		{0       , 0 ,  A , B},
		{0       , 0 , -1 , 0}
	};

	return Matrix4d(P);
}

Camera::Camera(Vector3d initialPos, Vector3d cameraOrientation, Vector3d up)
{
	this->up = up;
	this->cameraOrientation = cameraOrientation;
	this->translationVector = initialPos;
	this->currentType = CameraType::PERSPECTIVE;
	this->currentRotation = RotationMode::EULER;
	this->VboId[0] = 0;

	orthoMatrix = getOrthoProj(-2, 2, -2, 2, 1, 100);
	perspectiveMatrix = getPerspectiveProj(30, 640 / 640, 1, 100);
	initialView = MatrixFactory::translationMatrix(translationVector) * getRotationMatrix() * getViewMatrix();
}

Matrix4d Camera::getViewMatrix()
{
	Vector3d view = cameraOrientation;
	Vector3d v = view / (view.norm());
	Vector3d side = v ^ up;
	Vector3d s = side / (side.norm());
	Vector3d u = s ^ v;
	Matrix4d rotMatrix = Matrix4d(Vector4d(s), Vector4d(u), Vector4d(v) * (-1), Vector4d(0, 0, 0, 1));

	return rotMatrix;
}

Matrix4d Camera::getProjectionMatrix()
{
	if (this->currentType == CameraType::ORTHOGONAL) {
		return orthoMatrix;
	}
	else {
		return perspectiveMatrix;
	}
}

void Camera::applyRotation(float angleAroundX, float angleAroundY, float angleAroundZ)
{
	yaw += angleAroundY;
	pitch += angleAroundZ;
	roll += angleAroundX;
}

Matrix4d Camera::getRotationMatrix()
{
	if (this->currentRotation == RotationMode::EULER) {
		Matrix4d yawMatrix = MatrixFactory::rotateYMatrix(yaw);
		Matrix4d pitchMatrix = MatrixFactory::rotateZMatrix(pitch);
		Matrix4d rollMatrix = MatrixFactory::rotateXMatrix(roll);

		return yawMatrix * rollMatrix * pitchMatrix;
	}
	else {
		//q = q.fromEuler(-yaw, -pitch, -roll); //Gimbal Lock still present!
		Quaternion qX = Quaternion::fromAngleAxis(roll, Vector4d(1, 0, 0, 1));
		Quaternion qY = Quaternion::fromAngleAxis(yaw, Vector4d(0, 1, 0, 1));
		Quaternion qZ = Quaternion::fromAngleAxis(pitch, Vector4d(0, 0, 1, 1));
		Quaternion finalRot = qZ * qX * qY;
		return finalRot.toMatrix4d();
	}
}

void Camera::updateCameraPos(int key, int action)
{
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS) {
			keys[2] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[2] = false;
		}
	}
	else if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS) {
			keys[0] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[0] = false;
		}

	}
	
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS) {
			keys[1] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[1] = false;
		}
	}
	else if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS) {
			keys[3] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[3] = false;
		}
	}
}

void Camera::initBuffer()
{
	glGenBuffers(1, VboId);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[0]);
	{
		//tamanho da matriz 16 elementos * 2 por serem 2 matrizes
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 2, 0, GL_STREAM_DRAW);
		//TODO remove
		//glBindBufferBase(GL_UNIFORM_BUFFER, shaders.getUBO(), VboId[0]);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[0]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::destroyBuffer()
{
	glDeleteBuffers(1, VboId);
}

void Camera::draw()
{
	
	Matrix4d projMatrix = this->getProjectionMatrix();

	float proj_buffer[16];
	projMatrix.toColumnMajorArray(proj_buffer);

	float view_buffer[16];
	if (menu == 0) {
		Matrix4d viewMatrix = this->getViewMatrix();

		Matrix4d translationMatrix = MatrixFactory::translationMatrix(translationVector);
		viewMatrix = translationMatrix * getRotationMatrix() * viewMatrix;
		
		viewMatrix.toColumnMajorArray(view_buffer);
	}
	else {
		initialView.toColumnMajorArray(view_buffer);
	}


	glBindBuffer(GL_UNIFORM_BUFFER, VboId[0]);
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 16, view_buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16, sizeof(float) * 16, proj_buffer);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

float toRadians(float value) {
	return value * ((float) PI) / 180;
}

void Camera::look(float xpos, float ypos, bool pressed)
{
	if (!pressed) {
		was_pressed = false;
		return;
	}
	if (firstMouse || !was_pressed)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		was_pressed = true;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	this->applyRotation(yoffset, xoffset, 0);
}

void Camera::updateCamera() {
	if (keys[2])
	{
		Vector3d cameraSpeedVector = Vector3d(0, 0, CAMERA_SPEED);
		translationVector = translationVector - cameraSpeedVector;
	}
	else if (keys[0])
	{
		Vector3d cameraSpeedVector = Vector3d(0, 0, CAMERA_SPEED);
		translationVector = translationVector + cameraSpeedVector;
	}
}

void Camera::changeProjectionType() {
	if (currentType == CameraType::ORTHOGONAL) {
		currentType = CameraType::PERSPECTIVE;
	}
	else {
		currentType = CameraType::ORTHOGONAL;
	}
}

void Camera::changeRotationType()
{
	if (currentRotation == RotationMode::EULER) {
		std::cout << "CHANGE TO QUATERNION\n";
		currentRotation = RotationMode::QUATERNION;
	}
	else {
		std::cout << "CHANGE TO EULER\n";
		currentRotation = RotationMode::EULER;
	}
}

Vector3d Camera::getEulerAngles() {
	return Vector3d(roll,yaw,pitch);
}

void Camera::setEulerAngles(Vector3d angles)
{
	this->roll = angles.getX();
	this->yaw = angles.getY();
	this->pitch = angles.getZ();
}
