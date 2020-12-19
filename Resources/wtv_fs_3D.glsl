#version 330 core

//in process
uniform sampler3D NoiseTexture;
//uniform vec3 LightPosition;

uniform mat4 ModelMatrix;
//uniform mat3 NormalMatrix;
//
//uniform SharedMatrices {
//	mat4 ViewMatrix;
//	mat4 ProjectionMatrix;
//};

uniform float Levers[5];

in vec3 exVertex;
//in vec3 exNormal;
out vec4 FragmentColor;

const vec3 MarbleColor = vec3(0.5, 0.25, 0.3);
const vec3 VeinColor = vec3(0.85, 0.7, 0.75);
const float NoiseFactor = 3.0;
const float PositionFactor = 20;
const float IntensityFactor = 28;

void main(void)
{	
//	vec3 V = vec3(ViewMatrix * ModelMatrix * vec4(exVertex, 1.0));
//	vec3 N = normalize(NormalMatrix * exNormal);
//	vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition, 1.0));
//	vec3 L = normalize(Lpos - V);
//	float diffuse = max(dot(L,N), 0.0);
//
//	float specular = 0.0;
//	if (diffuse > 0.0) {
//		vec3 R = reflect(-L, N);
//		vec3 E = normalize(-V);
//		specular = pow(max(dot(R,E), 0.0), 32.0);
//	}

	vec3 wcPosition = vec3(ModelMatrix * vec4(exVertex, 1.0));
	vec3 p = wcPosition * 0.5 + 0.5;
	float noise = texture(NoiseTexture, p).r * 0.5 + 0.5;

	float NF = Levers[0] * NoiseFactor;
	float PF = Levers[1] * PositionFactor;
	float IF = Levers[2] * IntensityFactor;

	float vein_intensity = clamp(noise * NF, 0.0, 1.0);
	vein_intensity = cos(exVertex.x * PF + vein_intensity * IF) * 0.5 + 0.5;
	vec3 color = mix(MarbleColor, VeinColor, vein_intensity);

//	diffuse = diffuse * 0.75 + 0.25;
//	FragmentColor = vec4(color * diffuse + specular, 1.0);
	FragmentColor = vec4(vec3(color), 1.0);
}


