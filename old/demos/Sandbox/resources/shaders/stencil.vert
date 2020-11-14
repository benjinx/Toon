/*
    Stencil Shader
    By Benji Campbell
*/

#version 330 core

// attributes
layout (location = 0) in vec4 position;
// uniforms
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * position;
}