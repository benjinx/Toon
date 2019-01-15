/*
	Draw Color
	By Benji Campbell
	Fragment shader that displays color received from previous pipeline stage.
*/

#version 330 core

struct Material {
	sampler2D diffuseMap;
};

// varyings
in vertexData
{
	vec2 texCoords;
} pass;

// uniforms
uniform Material material;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(material.diffuseMap, pass.texCoords).rgb;
	fragColor = vec4(objectColor, 1.0); // use for diffuse
}
