#include "Texture3D.h"
#include "Noise.h"

#include <sstream>
#include <time.h>

void Texture3D::bind()
{
	glBindTexture(GL_TEXTURE_3D, id);
}

void Texture3D::unbind()
{
	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double scalez, const double alpha, const double beta, const int harmonics)
{
	srand((unsigned int)time(0));
	const unsigned int size = side * side * side;
	float* image = new float[size];
	int idx = 0;
	double step = 1.0 / (side - 1.0);
	for (double x = 0.0; x < 1.0; x += step)
	{
		for (double y = 0.0; y < 1.0; y += step)
		{
			for (double z = 0.0; z < 1.0; z += step)
			{
				PerlinNoise PN;
				double noise = PN.noise3D(scalex * x, scaley * y, scalez * z, alpha, beta, harmonics);
				image[idx++] = static_cast<float>(noise);
			}
		}
	}
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, side, side, side, 0, GL_RED, GL_FLOAT, image);
	glGenerateMipmap(GL_TEXTURE_3D);

	delete[] image;
}