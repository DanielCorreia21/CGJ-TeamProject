#include "Texture2D.h"
#include "Noise.h"

#include <sstream>
#include <time.h>

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::load(const std::string& filename)
{
	int width, height, channels;
	this->texture_path = filename;
	std::cout << " Loading Image" << filename;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		std::cout << " Error: Image is a nullptr" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << " Image loaded." << std::endl;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	//TODO why aren't the texture_wraps set?
	//filtro é o mesmo para a u e v coordenada
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
}

void Texture2D::loadMipMap(int levels, const std::string& prefix, const std::string& suffix)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	for (int i = 0; i < levels; ++i) {
		std::stringstream ss;
		ss << prefix << i << suffix;
		std::string filename = ss.str();

		int width, height;
		std::cout << "Loading mipmap" << filename;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		if (image == nullptr) {
			std::cout << "Error loading image while trying ot load mipmap" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			std::cout << "Loaded image succesfully for the mipmap" << std::endl;
		}
		glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double alpha, const double beta, const int harmonics)
{
	srand((unsigned int)time(0));
	const unsigned int size = side * side;
	float* image = new float[size];
	int idx = 0;
	double step = 1.0 / (double)side;
	for (double x = 0.0; x < 1.0; x += step)
	{
		for (double y = 0.0; y < 1.0; y += step)
		{
			PerlinNoise PN;
			double noise = PN.noise2D(scalex * x, scaley * y, alpha, beta, harmonics);
			image[idx++] = static_cast<float>(noise);
		}
	}
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, side, side, 0, GL_RED, GL_FLOAT, image);

	delete[] image;
}
