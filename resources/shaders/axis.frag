/*
	Draw Color
	By Benji Campbell
	Fragment shader that displays color received from previous pipeline stage.
*/

#version 330 core

// varying
in vertexData
{
	vec4 color;
} pass;


// ****
// target
layout (location = 0) out vec4 fragColor;

// shader entry point: function executes once per-fragment
void main()
{
	fragColor = pass.color;
}
