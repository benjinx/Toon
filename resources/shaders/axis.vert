/*
	Pass Color
	By Benji Campbell
	Vertex shader that passes attributes down pipeline.
*/

#version 330 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 5) in vec4 color;

// uniforms
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

//
out vertexData
{
	vec4 color;
} pass;

void main()
{
	gl_Position = projMat * viewMat  * modelMat * position;
	pass.color = color;
}