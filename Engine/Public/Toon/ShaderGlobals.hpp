#ifndef TOON_SHADER_GLOBALS_HPP
#define TOON_SHADER_GLOBALS_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>

namespace Toon {

#define TOON_SHADER_GLOBALS_BINDING 0

struct TOON_ENGINE_API ShaderGlobals
{
public:

    alignas(8) glm::vec2 Resolution;
    alignas(8) glm::vec2 Mouse;
    alignas(4) int FrameCount;
    alignas(4) float TotalTime;
    alignas(4) float FrameSpeedRatio;

}; // struct ShaderGlobals

} // namespace Toon

#endif // TOON_SHADER_GLOBALS_HPP