/*
    Draw Color
    By Benji Campbell
    Fragment shader that displays color received from previous pipeline stage.
*/

#version 330 core

// varying
in vertexData
{
    vec4 color;
} pass;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = pass.color;
}
