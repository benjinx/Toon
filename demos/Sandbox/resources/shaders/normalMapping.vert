#version 330 core

// Attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 tangent;

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
	vec2 texCoords;
	vec3 lightDir;
	vec3 eyeDir;
	vec3 normal;
} pass;

void main()
{
	pass.fragPos = vec3(modelMat * vec4(position.xyz, 1.0));
	pass.texCoords = vec2(texCoords.x, 1.0 - texCoords.y);

	mat3 worldSpaceMatrix = transpose(inverse(mat3(modelMat)));
	vec3 T = normalize(worldSpaceMatrix * tangent.xyz);
	vec3 N = normalize(worldSpaceMatrix * normal.xyz);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);// * tangent.w;

	mat3 TBN = mat3(T, B, N);

	vec3 tangentLightPos = lightPos.xyz * TBN;
	vec3 tangentEyePos = eyePos.xyz * TBN;
	vec3 tangentFragPos = pass.fragPos * TBN;
	pass.normal = N;

	pass.lightDir = normalize(tangentLightPos - tangentFragPos);
	pass.eyeDir = normalize(tangentEyePos - tangentFragPos);

	gl_Position =  mvp * vec4(position.xyz, 1.0);
}