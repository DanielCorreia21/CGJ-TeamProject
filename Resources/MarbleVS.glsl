#version 330 core

uniform mat4 ModelMatrix;

uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inVertex;
in vec2 inTexcoord;
in vec3 inNormal;

out vec2 exTexcoord;
out vec3 exVertex;
out vec3 exNormal;

void main(void)
{
	exVertex = inVertex;
	exNormal = inNormal;
	exTexcoord = inTexcoord.st;
	
	vec4 MCPosition = vec4(inVertex, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
