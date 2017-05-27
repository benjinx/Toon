/*
	Phong (VS)
	By Dan Buckstein
	Vertex shader that passes data required to perform Phong shading.
	
	Modified by: ______________________________________________________________
*/

// version
#version 330 core


// ****
// attributes
layout (location = 0) in vec3 position;
layout (location = 8) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 10) in vec3 tangent;
layout (location = 11) in vec3 bitangent;


// ****
// uniforms
uniform mat4 mvp;
uniform vec4 lightPos;
uniform vec4 eyePos;

// ****
// varyings

out vertexData
{
	vec2 uv;
	vec3 lightDir;
	vec3 viewDir;
} pass;

// shader function
void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	
	vec3 T = normalize(tangent);
	vec3 B = normalize(bitangent);
	vec3 N = normalize(normal);

	pass.uv = texCoord.xy;

	vec3 lightVec = (lightPos.xyz - position);
	vec3 eyeVec = (eyePos.xyz - position);

	pass.lightDir.x = dot(T, lightVec);
	pass.lightDir.y = dot(B, lightVec);
	pass.lightDir.z = dot(N, lightVec);
	
	pass.viewDir.x = dot(T, eyeVec);
	pass.viewDir.y = dot(B, eyeVec);
	pass.viewDir.z = dot(N, eyeVec);
}