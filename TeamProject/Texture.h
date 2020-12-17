#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Manager.h"

class Texture {

	protected:
		GLuint id;

	public:
		Texture();
		~Texture();
		virtual void bind() = 0;
		virtual void unbind() = 0;
};

class TextureManager : public Manager<Texture> {};