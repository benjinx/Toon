/*
    Pass Color
    By Benji Campbell
    Vertex shader that passes attributes down pipeline.
*/

#version 330 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 8) in vec2 texCoords;

// uniforms
uniform mat4 proj;

// varyings
out vec2 passTexCoord;

void main()
{
    gl_Position = proj * position;
    passTexCoord = texCoords;
}