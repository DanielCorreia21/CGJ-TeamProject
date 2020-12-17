#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Matrix4d.h"
#include "Manager.h"

/*	AUTHORS
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

class ShaderProgram
{
private:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint UniformId, UboId;

public:
	struct AttributeInfo {
		GLuint index;
	};
	std::map<std::string, AttributeInfo> attributes;

	struct UniformInfo {
		GLuint index;
	};
	std::map<std::string, UniformInfo> uniforms;

	struct UboInfo {
		GLuint index;
		GLuint binding_point;
	};
	std::map<std::string, UboInfo> ubos;

	ShaderProgram();
	void addAttribute(const std::string& name, const GLuint index);
	void addUniform(const std::string& name);
	void addUbo(const std::string& name, const GLuint binding_point);
	void init(const char vertexShaderPath[], const char fragmentShaderPath[], bool tcoords, bool normals);
	void bind();
	void unbind();
	void destroy();
	//void addUniformBlock(const std::string& name, const GLuint binding_point);
	GLint getUniformId();
	//GLint getUBO();
	//GLint getUniformColorId();
};

class ShaderProgramManager : public Manager<ShaderProgram> {};