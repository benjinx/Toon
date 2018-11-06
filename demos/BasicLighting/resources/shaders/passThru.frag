/*
	Draw Color
	By Benji Campbell
	Fragment shader that displays color received from previous pipeline stage.
*/

#version 330 core

// varyings
in vec2 passTexCoord;

// uniforms
uniform sampler2D diffuseTex;
uniform vec3 passColor;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
	//fragColor = texture(diffuseTex, passTexCoord); // use for diffuse
	fragColor = vec4(passColor, 1.0); // use for color
}
