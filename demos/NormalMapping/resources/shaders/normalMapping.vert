#version 330 core

// Attributes
layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;
layout (location = 8) in vec2 texCoords;
layout (location = 10) in vec4 tangent;
layout (location = 11) in vec4 bitangent;

// Uniforms
uniform mat4 mvp;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec4 lightPos;
uniform vec4 eyePos;

// Varyings
out vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 lightDir;
	vec4 eyeDir;
} pass;

void main()
{
	pass.fragPos = vec3(modelMat * vec4(position.xyz, 1.0));
	pass.texCoords = texCoords;

	vec3 T = normalize(mat3(modelMat) * tangent.xyz);
	vec3 B = normalize(mat3(modelMat) * bitangent.xyz);
	vec3 N = normalize(mat3(modelMat) * normal.xyz);

	vec3 lightVec = (lightPos.xyz - pass.fragPos);
	vec3 eyeVec = (eyePos.xyz - pass.fragPos);

	pass.lightDir.x = dot(T, lightVec);
	pass.lightDir.y = dot(B, lightVec);
	pass.lightDir.z = dot(N, lightVec);

	pass.eyeDir.x = dot(T, eyeVec);
	pass.eyeDir.y = dot(B, eyeVec);
	pass.eyeDir.z = dot(N, eyeVec);

	gl_Position =  projMat * viewMat * vec4(pass.fragPos, 1.0);
}