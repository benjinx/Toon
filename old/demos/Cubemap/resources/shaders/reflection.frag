#version 330 core

// Varyings
in vertexData
{
    vec3 fragPos;
    vec3 normal;
} pass;

// Uniforms
uniform vec3 eyePos;
uniform samplerCube cubemap;

// Targets
layout (location = 0) out vec4 FragColor;


void main()
{
    vec3 I = normalize(pass.fragPos - eyePos);
    vec3 R = reflect(I, normalize(pass.normal));
	FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}