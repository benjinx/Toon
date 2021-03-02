#ifndef TOON_GLOBALS_INC_GLSL
#define TOON_GLOBALS_INC_GLSL

layout(binding = 0, std140) uniform ToonGlobals
{
    vec2 Resolution;
    vec2 Mouse;
    int FrameCount;
    float TotalTime;
    float FrameSpeedRatio;

};

#endif // TOON_GLOBALS_INC_GLSL