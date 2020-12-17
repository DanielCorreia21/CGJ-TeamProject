#version 330 core

uniform sampler2D Texture_1;

in vec3 exPosition;
in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void)
{
	FragmentColor = texture(Texture_1, exTexcoord);
}