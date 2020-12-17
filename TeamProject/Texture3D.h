#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include <SOIL.h>

class Texture3D : public Texture
{
public:
	void bind() override;
	void unbind() override;
	void createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double scalez, const double alpha, const double beta, const int harmonics);
};