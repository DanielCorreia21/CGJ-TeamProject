#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Matrix4d.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class ShaderProgram
{
private:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint UniformId, UniformColorId, UboId;
public:
	ShaderProgram();
	void init(const char vertexShaderPath[], const char fragmentShaderPath[]);
	void bind();
	void unbind();
	void destroy();
	GLint getUniformId();
	GLint getUBO();
	GLint getUniformColorId();
	void setMat4(const std::string& name, const Matrix4d& mat) const;
};

