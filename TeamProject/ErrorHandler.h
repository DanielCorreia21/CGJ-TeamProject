#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class ErrorHandler
{
public:
	static const std::string errorString(GLenum error);
	static bool isOpenGLError();
	static void checkOpenGLError(std::string error);
};

