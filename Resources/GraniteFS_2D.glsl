#version 330 core

uniform sampler2D NoiseTexture;
uniform sampler2D Texture_1;

in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void)
{	

	float noise = texture(NoiseTexture, exTexcoord).r*0.5+0.5;
	vec4 N = vec4(vec3(noise),1.0);

	vec4 tex = texture(Texture_1, exTexcoord);

	FragmentColor = mix(tex, N, 0.4);

}
