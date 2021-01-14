#version 330 core

uniform sampler3D NoiseTexture;
uniform sampler2D Texture;
uniform vec4 Color;

uniform mat4 ModelMatrix;

in vec3 exVertex;
in vec2 exTexcoord;
out vec4 FragmentColor;

float random(in vec2 st) {
	return fract(sin(dot(st.xy,
		vec2(12.9898, 78.233)))
		* 43758.5453123);
}

float noise(vec2 st) {
	vec2 i = floor(st);
	vec2 f = fract(st);
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(mix(random(i + vec2(0.0, 0.0)),
		random(i + vec2(1.0, 0.0)), u.x),
		mix(random(i + vec2(0.0, 1.0)),
			random(i + vec2(1.0, 1.0)), u.x), u.y);
}

mat2 rotate2d(float angle) {
	return mat2(cos(angle), -sin(angle),
		sin(angle), cos(angle));
}

float smoothstep(float edge0, float edge1, float x) {
		float t;
    	t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    	return t * t * (3.0 - 2.0 * t);
}

float turbulence (vec2 P, int numFreq)
   {
      float val = 0.0;
      float freq = 1.0;
      for (int i=0; i<numFreq; i++) {
         val += abs (noise (P*freq) / freq);
         freq *= 2.07;
      }
      return val;
   }

float lines(in vec2 pos, float b) {
	float scale = 10.0;
	pos *= scale;
	return smoothstep(0.0,
		.5 + b * .5,
		abs((sin(pos.x * 3.1415) + b * 2.0)) * .5);
}

void main()
{
	if(Color == vec4(0,0,0,0)){
		FragmentColor = Color;
	}else{
		vec2 st = vec2(exVertex.x, exVertex.y);
		vec2 pos = st.yx * vec2(-0.5, 0.5);

		float pattern = pos.x;

		// Add noise
		pos = rotate2d(noise(pos)*25) * pos;

		//Add turbulence
		pos = turbulence(pos.xy,10) * pos;

		// Draw lines
		pattern = lines(pos*75, 1);

		FragmentColor = vec4(vec3(pattern * 0.3), 1.0) * vec4(0.5, 0.4, 0.3f,1.0f);
	}
}
