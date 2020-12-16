#include "TextureInfo.h"

TextureInfo::TextureInfo(GLenum textureunit, GLuint n_index, const std::string& n_uniform, Texture* n_texture)
{
	unit = textureunit;
	index = n_index;
	uniform = n_uniform;
	texture = n_texture;
}

void TextureInfo::updateShader(ShaderProgram* shader)
{
	glActiveTexture(unit);
	texture->bind();
	glUniform1i(shader->uniforms[uniform].index, index);
}
