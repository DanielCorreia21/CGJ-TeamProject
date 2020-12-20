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

class Texture3D : public Texture
{
public:
	void bind() override;
	void unbind() override;
	void createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double scalez, const double alpha, const double beta, const int harmonics);
};