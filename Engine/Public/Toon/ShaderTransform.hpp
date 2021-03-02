#ifndef TOON_SHADER_TRANSFORM_HPP
#define TOON_SHADER_TRANSFORM_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>

namespace Toon {

#define TOON_SHADER_TRANSFORM_BINDING 1

struct TOON_ENGINE_API ShaderTransform
{
public:
    alignas(64) glm::mat4 Model;
    alignas(64) glm::mat4 View;
    alignas(64) glm::mat4 Projection;
    alignas(64) glm::mat4 MVP;

    inline void UpdateMVP() {
        MVP = Projection * View * Model;
    }

}; // struct ShaderTransform

} // namespace Toon

#endif // TOON_SHADER_TRANSFORM_HPP