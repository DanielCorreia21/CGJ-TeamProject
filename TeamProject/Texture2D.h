#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include <SOIL.h>

class Texture2D : public Texture
{
	public:
		void bind() override;
		void unbind() override;
		void load(const std::string& filename);
		void loadMipMap(int levels, const std::string& prefix, const std::string& suffix);
		/*void createRandomNoise(const unsigned short side);
		void createPerlinNoise(const unsigned short side, const double scale_x, const double scale_y,
							   const double alpha, const double beta, const int harmonics);*/
};