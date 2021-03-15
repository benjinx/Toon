#ifndef VERTEX_ATTRIBUTES_INC_GLSL
#define VERTEX_ATTRIBUTES_INC_GLSL

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec4 a_Tangent;
layout(location = 3) in vec4 a_Bitangent;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TexCoord1;
layout(location = 6) in vec2 a_TexCoord2;
layout(location = 7) in uvec4 a_Joint;
layout(location = 8) in vec4 a_Weight;

#endif // VERTEX_ATTRIBUTES_INC_GLSL