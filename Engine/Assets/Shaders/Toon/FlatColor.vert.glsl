#version 450 core

#include <Toon/Globals.inc.glsl>
#include <Toon/Transform.inc.glsl>
#include <Toon/VertexAttributes.inc.glsl>

void main()
{
    gl_Position = u_MVP * a_Position;
}