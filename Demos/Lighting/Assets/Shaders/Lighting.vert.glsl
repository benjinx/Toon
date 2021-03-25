#version 450 core

#include <Toon/Transform.inc.glsl>
#include <Toon/VertexAttributes.inc.glsl>

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 5) out vec2 v_TexCoords;

void main()
{
    // Move the normal (perpendicular) to tangent space
    v_Normal = vec3(transpose(inverse(u_Model)) * a_Normal);

    vec4 pos = u_Model * a_Position;
    v_Position = pos.xyz / pos.w;

    //v_TexCoords = vec2(u_TexCoord1.x, 1.0 - u_TexCoord1.y);
    v_TexCoords = a_TexCoord1;

    gl_Position = u_MVP * a_Position;
}