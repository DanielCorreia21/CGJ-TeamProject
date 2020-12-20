#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Matrix4d.h"
#include "Manager.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class ShaderProgram
{
private:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint ModelMatrixIndex, UboId;

	void destroy();
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
	~ShaderProgram();
	void addAttribute(const std::string& name, const GLuint index);
	void addUniform(const std::string& name);
	void addUbo(const std::string& name, const GLuint binding_point);

	AttributeInfo getAttribute(const std::string& name);
	UniformInfo* getUniform(const std::string& name);
	UboInfo getUbo(const std::string& name);

	void init(const char vertexShaderPath[], const char fragmentShaderPath[]);
	void bind();
	void unbind();
	//void addUniformBlock(const std::string& name, const GLuint binding_point);
	GLint getModelMatrixIndex();
	//GLint getUBO();
	//GLint getUniformColorId();
};

class ShaderProgramManager : public Manager<ShaderProgram> {};