#version 330 core
uniform sampler3D NoiseTexture;

uniform mat4 ModelMatrix;

in vec3 inVertex;
out vec4 FragmentColor;

// definition of tiles, in meter:
const vec3 tileSize = vec3(1.1,1.0, 1.1);
const vec3 tilePct = vec3(0.98,1.0, 0.98);

 // Expects -1<x<1
vec3 marble_color (float x)
{
    vec3 col;
    x = 0.5*(x+1.);          // transform -1<x<1 to 0<x<1
    x = sqrt(x);             // make x fall of rapidly...
    x = sqrt(x);
    x = sqrt(x);
    col = vec3(.2 + .75*x);  // scale x from 0<x<1 to 0.2<x<0.95
    col.b*=0.95;             // slightly reduce blue component (make color "warmer"):
    return col;
}

float turbulence (vec3 P, int numFreq)
{   
    vec3 wcPosition = vec3(ModelMatrix * vec4(inVertex, 1.0));
	vec3 p = wcPosition * 0.5 + 0.5;
	float noise = texture(NoiseTexture, p).r * 0.5 + 0.5;

    float val = 0.0;
    float freq = 1.0;
    for (int i=0; i<numFreq; i++) {
        val += abs(noise(P*freq) / freq);
        freq *= 2.07;
    }
    return val;
}

void main(void)
{   
	vec4 MCPosition = vec4(inVertex, 1.0);

    float amplitude = 8.0;
    const int roughness = 4;     // noisiness of veins (#octaves in turbulence)

    float t = 6.28 * MCPosition.x / tileSize.x ;
    t += amplitude * turbulence (MCPosition.xyz, roughness);
    // replicate over rows of tiles (wont be identical, because noise is depending on all coordinates of the input vector):
    t = sin(t);
    vec3 marbleColor = marble_color(t);
    FragmentColor = vec4(marbleColor,1.0);

}