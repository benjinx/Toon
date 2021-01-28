#ifndef TOON_TRANSFORM_DATA_HPP
#define TOON_TRANSFORM_DATA_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>

namespace Toon {

struct TOON_ENGINE_API TransformData
{
public:
    alignas(64) glm::mat4 Model;
    alignas(64) glm::mat4 View;
    alignas(64) glm::mat4 Projection;
    alignas(64) glm::mat4 MVP;

    inline void UpdateMVP() {
        MVP = Projection * View * Model;
    }

}; // struct TransformData

} // namespace Toon

#endif // TOON_TRANSFORM_DATA_HPP