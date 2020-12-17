#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Texture.h"
using namespace std;

class TextureInfo {

	public:
		GLenum unit;
		GLuint index;
		std::string uniform;
		Texture* texture;

		TextureInfo(GLenum textureunit, GLuint index, const std::string& uniform, Texture* texture);
		void updateShader(ShaderProgram* shader);
};