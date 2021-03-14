#ifndef TOON_MATERIAL_INC_GLSL
#define TOON_MATERIAL_INC_GLSL

layout(binding = 2, std140) uniform ToonMaterial
{
    vec4 BaseColorFactor;
    vec4 EmissiveFactor;
    float MetallicFactor;
    float RoughnessFactor;
    float OcclusionStrength;
    float NormalScale;
};

layout (binding = 0) uniform sampler2D BaseColorMap;
layout (binding = 1) uniform sampler2D NormalMap;
layout (binding = 2) uniform sampler2D MetallicRoughnessMap;
layout (binding = 3) uniform sampler2D EmissiveMap;
layout (binding = 4) uniform sampler2D OcclusionMap;

#endif // TOON_MATERIAL_INC_GLSL