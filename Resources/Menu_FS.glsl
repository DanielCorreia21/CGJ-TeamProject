#version 330 core

uniform sampler2D Texture;
uniform vec4 Color;

in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void)
{
	if(Color == vec4(0,0,0,0)){
		FragmentColor = Color;
	}else{
		FragmentColor = texture(Texture, exTexcoord);
	}
}