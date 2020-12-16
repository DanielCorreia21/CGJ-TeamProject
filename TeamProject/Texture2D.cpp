#include "Texture2D.h"
#include <sstream>

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
	std::cout << "Loading Image" << filename;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		std::cout << "Error: Image is a nullptr" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "Image loaded." << std::endl;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	//filtro é o mesmo para a u e v coordenada
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
