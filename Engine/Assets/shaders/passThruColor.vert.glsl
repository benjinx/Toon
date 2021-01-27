/*
    Pass Color
    By Benji Campbell
    Vertex shader that passes attributes down pipeline.
*/

#version 450 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
//layout (location = 2) in vec2 texCoords;

// uniforms
layout (binding = 0, std140) uniform TransformData
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
    mat4 mvp;
};

// varyings
out vertexData
{
    vec4 norm;
} pass;

void main()
{
    pass.norm = normal;
    gl_Position = mvp * position;
    //gl_Position = position;
    //pass.texCoords = vec2(texCoords.x, 1.0 - texCoords.y);
}