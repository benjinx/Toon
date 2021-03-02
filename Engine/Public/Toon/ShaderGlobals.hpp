#ifndef TOON_SHADER_GLOBALS_HPP
#define TOON_SHADER_GLOBALS_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>

namespace Toon {

#define TOON_SHADER_GLOBALS_BINDING 0

struct LightData
{
    alignas(16) glm::vec4 Position;
    alignas(16) glm::vec4 Direction;
    alignas(16) glm::vec4 Color;
};

struct TOON_ENGINE_API ShaderGlobals
{
public:

    alignas(8) glm::vec2 Resolution;
    alignas(8) glm::vec2 Mouse;
    alignas(4) int FrameCount;
    alignas(4) float TotalTime;
    alignas(4) float FrameSpeedRatio;
    alignas(16) glm::vec4 CameraPosition;
    alignas(4) int LightCount;
    LightData Lights[10];

}; // struct ShaderGlobals

} // namespace Toon

#endif // TOON_SHADER_GLOBALS_HPP