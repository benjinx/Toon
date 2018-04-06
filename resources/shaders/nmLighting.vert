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
	mat4 modelMat;
	vec4 lightDir;
	vec4 eyeDir;
} pass;

void main()
{
	pass.fragPos = vec3(modelMat * vec4(position.xyz, 1.0));
	//pass.normal = mat3(transpose(inverse(modelMat))) * normal.xyz;
	pass.texCoords = texCoords;
	//pass.modelMat = modelMat;

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
//#version 410

//// Attributes
//layout (location = 0) in vec4 position;
//layout (location = 2) in vec4 normal;
//layout (location = 8) in vec2 texCoords;

//// Uniforms
//uniform mat4 modelMat;
//uniform mat4 mvp;
//uniform vec4 lightPos;
//uniform vec4 eyePos;

//// Varyings
//out vertexData
//{
//	vec4 fragPos;
//	vec4 normal;
//	vec4 lightPos;
//	vec4 eyePos;
//	vec2 texCoords;
//	mat4 modelMat;
//} pass;

//void main()
//{
//	gl_Position = mvp * position;

//	pass.fragPos = modelMat * position;
//	pass.normal = modelMat * normal;
//	pass.lightPos = lightPos;
//	pass.eyePos = eyePos;
//	pass.texCoords = texCoords;
//	pass.modelMat = modelMat;
//}

