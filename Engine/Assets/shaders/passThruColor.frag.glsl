/*
    Draw Color
    By Benji Campbell
    Fragment shader that displays color received from previous pipeline stage.
*/

#version 450 core

// varyings
// in vertexData
// {
//     vec2 texCoords;
// } pass;

// // uniforms
// uniform vec3 passColor;

// target
layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0, 1.0, 0.0, 1.0); // use for color
}