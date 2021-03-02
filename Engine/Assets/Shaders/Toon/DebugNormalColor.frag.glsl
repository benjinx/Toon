#version 450 core

#include <Toon/Globals.inc.glsl>

layout(location = 1) in vec4 v_Normal;

layout (location = 0) out vec4 o_Color;

void main()
{
    o_Color = vec4(abs(v_Normal.xyz), 1.0);
}
