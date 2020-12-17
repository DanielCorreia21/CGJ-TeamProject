#include <iostream>
#include <fstream>
#include "ShaderProgram.h"
#include "GLStatics.h"
#include "Vector4d.h"
#include "ErrorHandler.h"


//passar argumentos com UniformLocation ou Block
/*
in_position
in_texture
shared_matrices
model_matrix

in_color
*/
//new ones -> inhereted shaderProgram
//bindAttributeLocation tmb para todos?


//nesses casos getUniformId() etc ja nao guardamos do nosso lado?

ShaderProgram::ShaderProgram(){
	VertexShaderId = FragmentShaderId = ProgramId = UniformId = UboId = 0;
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

void ShaderProgram::init(const char vertexShaderPath[], const char fragmentShaderPath[], bool tcoords)
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
	glBindAttribLocation(ProgramId, VERTICES, "inVertex");

	if (tcoords) {
		glBindAttribLocation(ProgramId, TEXCOORDS, "inTexcoord");
	}

	glLinkProgram(ProgramId);
	//TODO ASIGN??!!
	this->UniformId = glGetUniformLocation(ProgramId, "ModelMatrix");
	this->UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, this->UboId, UBO_BP);


	std::map<std::string, AttributeInfo>::iterator it = this->attributes.begin();
	while (it != this->attributes.end())
	{
		glBindAttribLocation(ProgramId, it->second.index, it->first.c_str());
		it++;
	}

	//this->UniformColorId = glGetUniformLocation(ProgramId, "Color");
	std::map<std::string, UniformInfo>::iterator it2 = this->uniforms.begin();
	while (it2 != this->uniforms.end())
	{
		it2->second.index = glGetUniformLocation(ProgramId, it2->first.c_str());
		it2++;
	}

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

GLint ShaderProgram::getUniformId()
{
	return this->UniformId;
}

/*GLint ShaderProgram::getUBO()
{
	return UBO_BP;
}*/







