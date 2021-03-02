#ifndef TOON_TRANSFORM_INC_GLSL
#define TOON_TRANSFORM_INC_GLSL

layout(binding = 1, std140) uniform ToonTransform
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
    mat4 MVP;

};

#endif // TOON_TRANSFORM_INC_GLSL