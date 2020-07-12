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
    float ratio = 1.00 / 1.53;
    vec3 I = normalize(pass.fragPos - eyePos);
    vec3 R = refract(I, normalize(pass.normal), ratio);
	FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}