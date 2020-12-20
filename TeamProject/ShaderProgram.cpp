#include <iostream>
#include <fstream>
#include "ShaderProgram.h"
#include "GLStatics.h"
#include "Vector4d.h"
#include "ErrorHandler.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

ShaderProgram::ShaderProgram(){
	VertexShaderId = FragmentShaderId = ProgramId = ModelMatrixIndex = UboId = 0;
}

ShaderProgram::~ShaderProgram()
{
	std::cout << "\nDestroying shaders\n";
	this->destroy();
}

void ShaderProgram::addAttribute(const std::string& name, const GLuint index)
{
	AttributeInfo att;
	att.index = index;
	this->attributes.insert(std::make_pair(name, att));
}

void ShaderProgram::addUniform(const std::string& name)
{
	UniformInfo uni;
	//-1 by default, after binding will be updated
	uni.index = -1;
	this->uniforms.insert(std::make_pair(name, uni));
}

void ShaderProgram::addUbo(const std::string& name, const GLuint binding_point)
{
	UboInfo ubo;
	ubo.binding_point = binding_point;
	this->ubos.insert(std::make_pair(name, ubo));
}

ShaderProgram::AttributeInfo ShaderProgram::getAttribute(const std::string& name)
{
	return AttributeInfo();
}

ShaderProgram::UniformInfo* ShaderProgram::getUniform(const std::string& name)
{
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = this->uniforms.find(name);
	if (it != this->uniforms.end()) {
		return &(it->second);
	}
	return nullptr;
}

ShaderProgram::UboInfo ShaderProgram::getUbo(const std::string& name)
{
	return UboInfo();
}

std::string sVertexShader;
std::string sFragmentShader;

void readVertexShader(const char vertexShaderPath[]) {
	std::ifstream ifs(vertexShaderPath);
	std::string temp((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	sVertexShader = temp;
}

void readFragmentShader(const char fragmentShaderPath[]) {

	std::ifstream ifs(fragmentShaderPath);
	std::string temp((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	sFragmentShader = temp;
}

void ShaderProgram::init(const char vertexShaderPath[], const char fragmentShaderPath[])
{
	readVertexShader(vertexShaderPath);
	readFragmentShader(fragmentShaderPath);
	const char* VertexShader = sVertexShader.c_str();
	const char* FragmentShader = sFragmentShader.c_str();


	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	//--------------------------------------------------------------------------------------
	//Default attributes
	glBindAttribLocation(ProgramId, VERTICES, "inVertex");
	glBindAttribLocation(ProgramId, TEXCOORDS, "inTexcoord");
	glBindAttribLocation(ProgramId, NORMALS, "inNormal");


	//default uniform
	glLinkProgram(ProgramId);
	this->ModelMatrixIndex = glGetUniformLocation(ProgramId, "ModelMatrix");

	//default UBO
	this->UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, this->UboId, UBO_BP);


	//bind new attributes
	std::map<std::string, AttributeInfo>::iterator it = this->attributes.begin();
	while (it != this->attributes.end())
	{
		glBindAttribLocation(ProgramId, it->second.index, it->first.c_str());
		it++;
	}

	//bind new uniforms
	std::map<std::string, UniformInfo>::iterator it2 = this->uniforms.begin();
	while (it2 != this->uniforms.end())
	{
		it2->second.index = glGetUniformLocation(ProgramId, it2->first.c_str());
		it2++;
	}

	//bind new UBO's
	std::map<std::string, UboInfo>::iterator it3 = this->ubos.begin();
	while (it3 != this->ubos.end())
	{
		it3->second.index = glGetUniformBlockIndex(ProgramId, it3->first.c_str());
		glUniformBlockBinding(ProgramId, it3->second.index, it3->second.binding_point);
		it3++;
	}
	//--------------------------------------------------------------------------------------

	glDetachShader(ProgramId, VertexShaderId);
	glDeleteShader(VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);

#ifndef ERROR_CALLBACK
	ErrorHandler::checkOpenGLError("ERROR: Could not create shaders.");
#endif
}

void ShaderProgram::bind()
{
	glUseProgram(ProgramId);
}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}

void ShaderProgram::destroy()
{
		glUseProgram(0);
		glDeleteProgram(ProgramId);

#ifndef ERROR_CALLBACK
		ErrorHandler::checkOpenGLError("ERROR: Could not destroy shaders.");
#endif
}

GLint ShaderProgram::getModelMatrixIndex()
{
	return this->ModelMatrixIndex;
}







