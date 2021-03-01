#version 450 core

// Attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

// uniforms
layout (binding = 0, std140) uniform TransformData
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
    mat4 mvp;
};

// Varyings
layout (location = 0) out vertexData
{
    vec3 fragPos;
    vec3 normal;
} pass;

void main()
{
    pass.fragPos = vec3(Model * vec4(position.xyz, 1.0));
    pass.normal = mat3(transpose(inverse(Model))) * normal.xyz;

    gl_Position =  Proj * View * vec4(pass.fragPos, 1.0);
}