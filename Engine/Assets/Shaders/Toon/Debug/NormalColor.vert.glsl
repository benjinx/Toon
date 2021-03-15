#version 450 core

#include <Toon/Globals.inc.glsl>
#include <Toon/Transform.inc.glsl>
#include <Toon/VertexAttributes.inc.glsl>

layout (location = 0) out vec4 v_Normal;

void main()
{
    gl_Position = u_MVP * a_Position;
    v_Normal = a_Normal;
}