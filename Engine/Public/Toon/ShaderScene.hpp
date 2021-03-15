#ifndef TOON_SHADER_SCENE_HPP
#define TOON_SHADER_SCENE_HPP

#include <Toon/Math.hpp>

namespace Toon {

#define TOON_MAX_LIGHT_COUNT 10

struct LightData1
{
    alignas(16) glm::vec3 Position;
    alignas(4)  uint8_t Type;
    alignas(16) glm::vec3 Direction;
    alignas(4)  float InnerCutOff;
    alignas(16) glm::vec3 Color;
    alignas(4)  float OuterCutOff;
}; // struct LightData

struct ShaderScene
{
    glm::vec3 CameraPosition;

    glm::vec3 AmbientLightColor;
    glm::vec3 AmbientLightDirection;

    LightData Lights[TOON_MAX_LIGHT_COUNT];
}; // struct ShaderScene

} // namespace Toon

#endif // TOON_SHADER_SCENE_HPP