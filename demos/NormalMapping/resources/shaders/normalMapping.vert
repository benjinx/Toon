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
	vec3 lightDir;
	vec3 eyeDir;
} pass;

void main()
{
	pass.fragPos = vec3(modelMat * vec4(position.xyz, 1.0));
	pass.texCoords = texCoords;

	mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
	vec3 T = normalize(normalMatrix * tangent.xyz);
	vec3 N = normalize(normalMatrix * normal.xyz);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));

	vec3 tangentLightPos = TBN * lightPos.xyz;
	vec3 tangentEyePos = TBN * eyePos.xyz;
	vec3 tangentFragPos = TBN * pass.fragPos;

	pass.lightDir = normalize(tangentLightPos - tangentFragPos);
	pass.eyeDir = normalize(tangentEyePos - tangentFragPos);

	gl_Position =  projMat * viewMat * modelMat * vec4(position.xyz, 1.0);
}