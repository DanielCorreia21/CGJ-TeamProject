#include <iostream>
#include <fstream>
#include "ShaderProgram.h"
#include "GLStatics.h"
#include "Vector4d.h"
#include "ErrorHandler.h"

//estrutura de vectores e matrizes melhor glsl ou *glm*

//passar argumentos com UniformLocation ou Block
/*
in_posistion
in_color
in_texture
shared_matrices
model_matrix
*/
//new ones -> inhereted shaderProgram
//bindAttributeLocation tmb para todos?


//UBO_BP should be defined in a nother statics class
//nesses casos getUniformId() etc ja nao guardamos do nosso lado?

ShaderProgram::ShaderProgram(){
	VertexShaderId = FragmentShaderId = ProgramId = UniformId = UniformColorId = UboId = 0;
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
	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");

	glLinkProgram(ProgramId);
	this->UniformId = glGetUniformLocation(ProgramId, "ModelMatrix");
	this->UniformColorId = glGetUniformLocation(ProgramId, "Color");
	this->UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, this->UboId, UBO_BP);
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

GLint ShaderProgram::getUBO()
{
	return UBO_BP;
}

GLint ShaderProgram::getUniformColorId()
{
	return this->UniformColorId;
}

void ShaderProgram::setMat4(const std::string& name, const Matrix4d& mat) const
{
	float opengl_mat[16];
	mat.toColumnMajorArray(opengl_mat);
	glUniformMatrix4fv(glGetUniformLocation(UniformId, name.c_str()), 1, GL_FALSE, opengl_mat);
}





