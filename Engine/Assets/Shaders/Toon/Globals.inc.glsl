#ifndef TOON_GLOBALS_INC_GLSL
#define TOON_GLOBALS_INC_GLSL

struct LightData
{
    vec3 Position;
    vec3 Direction;
    vec3 Color;
};

layout(binding = 0, std140) uniform ToonGlobals
{
    vec2 u_Resolution;
    vec2 u_Mouse;
    int u_FrameCount;
    float u_TotalTime;
    float u_FrameSpeedRatio;
    vec3 CameraPosition;
    int LightCount;
    LightData Lights[10];
};

#endif // TOON_GLOBALS_INC_GLSL