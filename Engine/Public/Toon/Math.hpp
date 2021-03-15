#ifndef MATH_HPP
#define MATH_HPP

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RIGHT_HANDED
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <limits>

namespace Toon {

using glm::quat;

using glm::mat2;
using glm::mat3;
using glm::mat4;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::bvec2;
using glm::bvec3;
using glm::bvec4;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::uvec2;
using glm::uvec3;
using glm::uvec4;

template <typename T>
constexpr float NormalizeInteger(T value)
{
    return (
        value < 0 
        ? -static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::min())
        :  static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::max())
    );
}

} // namespace Toon

#endif // MATH_HPP