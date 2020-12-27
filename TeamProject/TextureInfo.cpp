#include "TextureInfo.h"

/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	André Santos - 91000
*/

TextureInfo::TextureInfo(GLenum textureunit, GLuint n_index, const std::string& n_uniform, Texture* n_texture)
{
	unit = textureunit;
	index = n_index;
	uniform = n_uniform;
	texture = n_texture;
	texturePath = n_texture->texture_path;
}

void TextureInfo::updateShader(ShaderProgram* shader)
{
	glActiveTexture(unit);
	texture->bind();
	glUniform1i(shader->uniforms[uniform].index, index);
}
