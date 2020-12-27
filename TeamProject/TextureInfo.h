#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Texture.h"
using namespace std;

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class TextureInfo {

	public:
		GLenum unit;
		GLuint index;
		std::string uniform;
		Texture* texture;
		std::string texturePath;

		TextureInfo(GLenum textureunit, GLuint index, const std::string& uniform, Texture* texture);
		void updateShader(ShaderProgram* shader);
};