#version 450 core

#include <Toon/Transform.inc.glsl>
#include <Toon/VertexAttributes.inc.glsl>

layout (location = 0) out vec4 v_Position;
layout (location = 1) out vec4 v_Normal;

void main()
{
    // Move the normal to world space
    v_Normal = transpose(inverse(Model)) * a_Normal;

    v_Position = Model * a_Position;

    gl_Position = MVP * a_Position;
}