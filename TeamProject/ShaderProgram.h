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
using namespace std;

class ShaderProgram : public Mappable
{
private:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint ModelMatrixIndex, UboId;
	string sVertexShader;
	string sFragmentShader;
	void readVertexShader(const char vertexShaderPath[]);
	void readFragmentShader(const char fragmentShaderPath[]);
	void destroy();
public:
	struct AttributeInfo {
		GLuint index;
	};
	map<string, AttributeInfo> attributes;

	struct UniformInfo {
		GLuint index;
	};
	map<string, UniformInfo> uniforms;

	struct UboInfo {
		GLuint index;
		GLuint binding_point;
	};
	map<string, UboInfo> ubos;
	string vertexPath;
	string fragmentPath;

	ShaderProgram();
	~ShaderProgram();
	void addAttribute(const string& name, const GLuint index);
	void addUniform(const string& name);
	void addUbo(const string& name, const GLuint binding_point);

	AttributeInfo getAttribute(const string& name);
	UniformInfo* getUniform(const string& name);
	UboInfo getUbo(const string& name);

	void init(const char vertexShaderPath[], const char fragmentShaderPath[]);
	void bind();
	void unbind();
	//void addUniformBlock(const std::string& name, const GLuint binding_point);
	GLint getModelMatrixIndex();
	//GLint getUBO();
	//GLint getUniformColorId();
};

class ShaderProgramManager : public Manager<ShaderProgram> {};