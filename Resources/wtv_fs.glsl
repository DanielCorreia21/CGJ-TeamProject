#version 330 core

uniform sampler2D NoiseTexture;

in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void)
{
	float noise = texture(NoiseTexture, exTexcoord).r * 0.5 + 0.5;
	FragmentColor = vec4(vec3(noise), 1.0);
}


