/*
    Draw Color
    By Benji Campbell
    Fragment shader that displays color received from previous pipeline stage.
*/

#version 450 core

// varyings
layout (location = 0) in vertexData
{
    vec4 norm;
} pass;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(abs(pass.norm.xyz), 1.0);
}
