#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include <SOIL.h>

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

class Texture2D : public Texture
{
	public:
		void bind() override;
		void unbind() override;
		void load(const std::string& filename);
		void loadMipMap(int levels, const std::string& prefix, const std::string& suffix);
		void createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double alpha, const double beta, const int harmonics);
		std::string texture_path;
};