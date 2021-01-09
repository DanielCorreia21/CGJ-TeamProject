#version 330 core

uniform sampler3D NoiseTexture;

uniform mat4 ModelMatrix;

in vec3 exVertex;
in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void)
{	
	vec4 noise = texture(NoiseTexture, 2.0 * exVertex);

	FragmentColor = noise;

}
