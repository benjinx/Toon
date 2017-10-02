#version 330 core

// Attributes
layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;
layout (location = 8) in vec2 texCoords;

// Uniforms
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
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
	mat4 modelMat;
} pass;

void main()
{
	pass.fragPos = modelMat * position;
	pass.normal = transpose(inverse(modelMat)) * normal;
	pass.lightPos = lightPos - pass.fragPos;
	pass.eyePos = eyePos - pass.fragPos;
	pass.texCoords = texCoords.xy;
	pass.modelMat = modelMat;

	gl_Position = projMat * viewMat * pass.fragPos;
}