#pragma once

#include <string>
#include "ShaderProgram.h"

namespace engine
{
	class Texture;
	class Texture2D;
	class Texture3D;
	struct TextureInfo;
	class TextureManager;

	class Sampler;

	class Texture
	{
	protected:
		GLuint id;

	public:
		Texture();
		~Texture();
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	struct TextureInfo
	{
		GLenum unit;
		GLuint index;
		std::string uniform;
		Texture* texture = nullptr;
		Sampler* sampler = nullptr;
		TextureInfo(GLenum textureunit, GLuint index, const std::string& uniform, Texture* texture, Sampler* sampler);
		void updateShader(ShaderProgram* shader);
	};

	class Texture2D : public Texture
	{
	public:
		void bind() override;
		void unbind() override;
		void load(const std::string& filename);
		void loadMipMap(int levels, const std::string& prefix, const std::string& suffix);
		void createPerlinNoise(const unsigned short side, const double scalex, const double scaley, const double alpha, const double beta, const int harmonics);
	};

	class Sampler
	{
	protected:
		GLuint _samplerId;

	public:
		Sampler();
		~Sampler();
		virtual void create() = 0;
		void bind(GLuint unit);
		void unbind(GLuint unit);
	};
}