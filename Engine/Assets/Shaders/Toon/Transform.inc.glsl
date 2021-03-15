#ifndef TOON_TRANSFORM_INC_GLSL
#define TOON_TRANSFORM_INC_GLSL

layout(binding = 1, std140) uniform ToonTransform
{
    mat4 u_Model;
    mat4 u_View;
    mat4 u_Proj;
    mat4 u_MVP;

};

#endif // TOON_TRANSFORM_INC_GLSL