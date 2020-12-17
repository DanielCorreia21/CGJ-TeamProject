#include "texture.h"
#include "noise.h"
#include "soil.h"

#include<cassert>
#include<time.h>
#include<sstream>
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace engine
{
	//////TEXTURE INFO

	TextureInfo::TextureInfo(GLenum _unit, GLuint _index, const std::string& _uniform, Texture* _texture, Sampler* _sampler) :
		unit(_unit), uniform(_uniform), index(_index)
	{
		texture = _texture;
		sampler = _sampler;
	}

	void TextureInfo::updateShader(ShaderProgram* shader)
	{
		glActiveTexture(unit);
		texture->bind();
		GLint loc = shader->uniforms[uniform].index;
		glUniform1i(loc, index);
		if (sampler) sampler->bind(index);
	}

	//////TEXTURE

	Texture::Texture() :
		id(-1)
	{
	}
	Texture::~Texture()
	{
	}

	//////TEXTURE 1D

	//////TEXTURE 2D
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
		std::cout << "Loading image file " << filename << "... ";
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
		if (image == nullptr)
		{
			std::cout << "error" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			std::cout << "ok." << std::endl;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

		for (int i = 0; i < levels; ++i)
		{
			std::stringstream ss;
			ss << prefix << i << suffix;
			std::string filename = ss.str();

			int width, height;
			std::cout << "Loading mipmap file " << filename << "... ";
			unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
			if (image == nullptr)
			{
				std::cout << "error" << std::endl;
				exit(EXIT_FAILURE);
			}
			else {
				std::cout << "ok." << std::endl;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, side, side, 0, GL_RED, GL_FLOAT, image);

		delete[] image;
	}
}