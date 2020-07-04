/*
    Pass UV/TexCoords as color
    By Benji Campbell
    Vertex shader that passes attributes down pipeline.
*/

#version 330 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 texCoords;

// uniforms
uniform mat4 mvp;

// varyings
out vertexData
{
    vec2 texCoords;
} pass;

void main()
{
    gl_Position = mvp * position;
    pass.texCoords = vec2(texCoords.x, 1.0 - texCoords.y);
}