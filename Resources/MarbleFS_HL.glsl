#version 330 core

#define Integral(x, p, np) ((floor(x)*(p)) + max(fract (x) - (np), 0.0))

uniform sampler3D NoiseTexture;
uniform vec4 Color;

uniform mat4 ModelMatrix;

in vec3 exVertex;
out vec4 FragmentColor;

//vec3 rand(vec3 p){ 
    
//    float n = sin(dot(p, vec3(7, 157, 113)));    
//    return fract(vec3(2097152, 262144, 32768)*n); 
//}

const vec3 tileSize = vec3(1.1,1.0, 1.1);
const vec3 tilePct = vec3(0.98,1.0, 0.98);

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

void main(void)
{   
    if(Color == vec4(0,0,0,0)){
		FragmentColor = Color;
	}else{

//         // Get tile number - this adapts tileSize, transforming 0..tileSize to 0..1.
//         // (factor 16 comes from vs and should be removed at both ends!):
//         vec3 Tpos = exVertex / tileSize;
//
//         // move each other row of tiles:
//         if (fract (Tpos.x*0.5) > 0.5)
//           Tpos.z += 0.5;
//
//         // Make position relative to tile:
//         vec3 pos = fract (Tpos);

         // --- Calculate the marble color ---
         const int roughness = 4;     // noisiness of veins (#octaves in turbulence)

         //vec3 tileID = ceil(Tpos); // get ID of tile, unique to a tile and common to all its pixels
         float asc= 3* noise (2.3*(exVertex.xy));  // use this as m in t=my+x, rather than just using t=x.

         const float PI = 3.1415;
         float amplitude = 6.0;
         float t = 2.0*PI*(exVertex.x + (asc*exVertex.z)) / tileSize.x ;
         t += amplitude* turbulence (exVertex.xy, roughness);
         // replicate over rows of tiles (wont be identical, because noise is depending on all coordinates of the input vector):
         t = sin(t);
         vec3 marbleColor = marble_color(t);
  
         // get filter size:
         //vec3 fw = fwidth (exVertex);
  
         // Determine if marble or joint: isMarble will be 0 if there is marble and 1 if we are in a joint
         // vec3 isMarble = step (pos, tilePct);
         //vec3 isMarble = (Integral (pos+fw, tilePct, 1.-tilePct) - Integral (pos, tilePct, 1.-tilePct)) / fw;

         // mix the two colors together, isMarble decides which color to use:
         vec3 color = mix (vec3 (0.2, 0.2, 0.2), marbleColor, 0.5);

         FragmentColor = vec4(color, 1.0);
    }

}