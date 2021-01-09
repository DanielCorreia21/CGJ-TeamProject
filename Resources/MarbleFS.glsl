#version 330 core

uniform sampler3D NoiseTexture;
uniform sampler2D Texture_1;

uniform mat4 ModelMatrix;

in vec3 exVertex;
in vec2 exTexcoord;
out vec4 FragmentColor;

const vec3 colour1 = vec3(0.5, 0.25, 0.3);
const vec3 colour2 = vec3(0.85, 0.7, 0.75);

void main(void)
{	

	vec3 wcPosition = vec3(ModelMatrix * vec4(exVertex, 1.0));
	vec3 p = wcPosition * 0.5 + 0.5;
	float noise = texture(NoiseTexture, 0.5 * p).r * 0.5 + 0.5;

	vec4 tex = texture(Texture_1, exTexcoord);

	float intensity = clamp(noise, 0.0, 1.0);
	intensity = cos(exVertex.x + intensity*2)*0.5 + 0.5;
	vec3 color = mix(colour1, colour2, intensity);
	FragmentColor = mix(tex, vec4(color,1.0), 0.8);

}
