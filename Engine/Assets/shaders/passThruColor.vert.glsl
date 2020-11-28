/*
    Pass Color
    By Benji Campbell
    Vertex shader that passes attributes down pipeline.
*/

#version 450 core

// attributes
layout (location = 0) in vec4 position;
//layout (location = 2) in vec2 texCoords;

// uniforms
//uniform mat4 mvp;

// varyings
// out vertexData
// {
//     vec2 texCoords;
// } pass;

void main()
{
    //gl_Position = mvp * position;
    gl_Position = position;
    //pass.texCoords = vec2(texCoords.x, 1.0 - texCoords.y);
}