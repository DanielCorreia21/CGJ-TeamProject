#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Manager.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/


class Texture {

	protected:
		GLuint id;

	public:
		Texture();
		~Texture();
		virtual void bind() = 0;
		virtual void unbind() = 0;
		std::string texture_path = "";
};

class TextureManager : public Manager<Texture> {};