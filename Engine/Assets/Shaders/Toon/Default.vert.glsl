#version 450 core

#include <Toon/TBN.inc.glsl>
#include <Toon/Transform.inc.glsl>
#include <Toon/VertexAttributes.inc.glsl>

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out mat3 v_TBN;

void main()
{
    vec4 postion = u_Model * a_Position;
    v_Position = a_Position.xyz / a_Position.w;

    v_TexCoord = a_TexCoord1;

    v_TBN = CalculateTBN(u_Model, a_Normal, a_Tangent);

    gl_Position = u_MVP * a_Position;
}