#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class ErrorHandler
{
public:
	static const std::string errorString(GLenum error);
	static bool isOpenGLError();
	static void checkOpenGLError(std::string error);
};

