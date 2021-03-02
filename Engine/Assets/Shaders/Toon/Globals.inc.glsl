#ifndef TOON_GLOBALS_INC_GLSL
#define TOON_GLOBALS_INC_GLSL

struct LightData
{
    vec4 Position;
    vec4 Direction;
    vec4 Color;
};

layout(binding = 0, std140) uniform ToonGlobals
{
    vec2 Resolution;
    vec2 Mouse;
    int FrameCount;
    float TotalTime;
    float FrameSpeedRatio;
    vec4 CameraPosition;
    int LightCount;
    LightData Lights[10];
};

#endif // TOON_GLOBALS_INC_GLSL