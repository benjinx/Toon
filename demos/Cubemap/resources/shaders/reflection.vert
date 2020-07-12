#version 330 core

// Attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

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
} pass;

void main()
{
    pass.normal = mat3(transpose(inverse(modelMat))) * normal;

    pass.fragPos = vec3(modelMat * vec4(position, 1.0));
	gl_Position = projMat * viewMat * vec4(pass.fragPos, 1.0);
}