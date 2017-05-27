/*
	Pass Color
	By Benji Campbell
	Vertex shader that passes attributes down pipeline.
*/

#version 330 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;
layout (location = 8) in vec2 texCoord;

// uniforms
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 mvp;
uniform vec2 resolution;

// varyings
out vec2 passTexCoord;
out mat4 passVM;

void main()
{
	vec4 screenCoord = mvp * position;
	gl_Position = screenCoord;
	screenCoord /= screenCoord.w; // perspective divide

	

	vec2 uv;
	uv.x = screenCoord.x / normalize(resolution.x);
	uv.y = screenCoord.y / normalize(resolution.y);

	uv.y *= resolution.y / resolution.x;
	passTexCoord = uv.xy;
	passVM = viewMat;

	//gl_Position = mvp * position;
}