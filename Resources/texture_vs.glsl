#version 330 core

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
in vec2 inTexcoord;
out vec3 exPosition;
out vec2 exTexcoord;

void main(void)
{
	exPosition = inVertex;
	exTexcoord = inTexcoord;
	
	vec4 MCPosition = vec4(inVertex, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}