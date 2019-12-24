#version 410

// Attributes
layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;
layout (location = 8) in vec2 texCoords;

// Uniforms
uniform mat4 modelMat;
uniform mat4 mvp;
uniform vec4 lightPos;
uniform vec4 eyePos;

// Varyings
out vertexData
{
	vec4 fragPos;
	vec4 normal;
	vec4 lightPos;
	vec4 eyePos;
	vec2 texCoords;
} pass;

void main()
{
	gl_Position = mvp * position;

	pass.fragPos = modelMat * position;
	pass.normal = modelMat * normal;
	pass.lightPos = lightPos;
	pass.eyePos = eyePos;
	pass.texCoords = texCoords;
}