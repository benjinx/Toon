/*
    Pass Color
    By Benji Campbell
    Vertex shader that passes attributes down pipeline.
*/

#version 450 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

// uniforms
layout (binding = 1, std140) uniform TransformData
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
    mat4 mvp;
};

// varyings
layout (location = 0) out vertexData
{
    vec4 norm;
} pass;

void main()
{
    pass.norm = normal;
    gl_Position = mvp * position;
}