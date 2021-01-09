#version 330 core

uniform sampler3D NoiseTexture;

uniform mat4 ModelMatrix;

in vec3 exVertex;
in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void)
{	

	vec3 wcPosition = vec3(ModelMatrix * vec4(exVertex, 1.0));
	vec3 p = wcPosition * 0.5 + 0.5;
	float noise = texture(NoiseTexture, p).r * 0.5 + 0.5;

	FragmentColor = vec4(vec3(noise), 1.0);

}
