#version 450 core

#include <Toon/Transform.inc.glsl>
#include <Toon/VertexAttributes.inc.glsl>

layout (location = 0) out vec4 v_Position;
layout (location = 1) out vec4 v_Normal;
layout (location = 5) out vec2 v_TexCoords;

void main()
{
    // Move the normal to world space
    v_Normal = transpose(inverse(Model)) * a_Normal;

    v_Position = Model * a_Position;

    //v_TexCoords = vec2(a_UV1.x, 1.0 - a_UV1.y);
    v_TexCoords = a_UV1;

    gl_Position = MVP * a_Position;
}