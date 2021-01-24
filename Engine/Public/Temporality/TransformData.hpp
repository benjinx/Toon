#ifndef TEMPORALITY_TRANSFORM_DATA_HPP
#define TEMPORALITY_TRANSFORM_DATA_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Math.hpp>

namespace Temporality {

struct TEMPORALITY_ENGINE_API TransformData
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

} // namespace Temporality

#endif // TEMPORALITY_TRANSFORM_DATA_HPP