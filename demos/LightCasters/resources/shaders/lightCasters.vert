#version 330 core

// Attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoords;

// Uniforms
uniform mat4 mvp;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

// Varyings
out vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} pass;

void main()
{
	pass.fragPos = vec3(modelMat * vec4(position.xyz, 1.0));
	pass.normal = mat3(transpose(inverse(modelMat))) * normal.xyz;
	pass.texCoords = vec2(texCoords.x, 1.0 - texCoords.y);

	gl_Position =  projMat * viewMat * vec4(pass.fragPos, 1.0);
}
