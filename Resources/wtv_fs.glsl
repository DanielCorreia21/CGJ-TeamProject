#version 330 core

uniform sampler2D NoiseTexture;

uniform float Levers[5];

in vec3 exVertex;
in vec2 exTexcoord;
out vec4 FragmentColor;

const vec3 MarbleColor = vec3(0.5, 0.25, 0.3);
const vec3 VeinColor = vec3(0.85, 0.7, 0.75);
const float NoiseFactor = 3.0;
const float PositionFactor = 20;
const float IntensityFactor = 28;

void main(void)
{

	float noise = texture(NoiseTexture, exTexcoord).r * 0.5 + 0.5;

	float NF = Levers[0] * NoiseFactor;
	float PF = Levers[1] * PositionFactor;
	float IF = Levers[2] * IntensityFactor;

	float vein_intensity = clamp(noise * NF, 0.0, 1.0);
	vein_intensity = cos(exVertex.x * PF + vein_intensity * IF) * 0.5 + 0.5;
	vec3 color = mix(MarbleColor, VeinColor, vein_intensity);

	FragmentColor = vec4(vec3(color), 1.0);
}


