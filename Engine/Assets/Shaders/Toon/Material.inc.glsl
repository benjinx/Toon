#ifndef TOON_MATERIAL_INC_GLSL
#define TOON_MATERIAL_INC_GLSL

layout(binding = 2, std140) uniform ToonMaterial
{
    vec4 u_BaseColorFactor;
    vec3 u_EmissiveFactor;
    float u_MetallicFactor;
    float u_RoughnessFactor;
    float u_OcclusionStrength;
    float u_NormalScale;
};

layout (binding = 0) uniform sampler2D u_BaseColorMap;
layout (binding = 1) uniform sampler2D u_NormalMap;
layout (binding = 2) uniform sampler2D u_MetallicRoughnessMap;
layout (binding = 3) uniform sampler2D u_EmissiveMap;
layout (binding = 4) uniform sampler2D u_OcclusionMap;

#endif // TOON_MATERIAL_INC_GLSL