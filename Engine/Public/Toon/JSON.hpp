#ifndef TOON_JSON_HPP
#define TOON_JSON_HPP

#include <Toon/Config.hpp>
#include <Toon/String.hpp>

#include <nlohmann/json.hpp>

namespace Toon {

typedef nlohmann::basic_json<
    std::map,
    std::vector,
    string,
    bool,
    int,
    unsigned,
    float,
    std::allocator> json;

inline glm::vec2 ExtractVec2(const json& value, glm::vec2 def)
{
    if (value.is_array() && value.size() == 2) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec2(v.data());
    }

    return def;
}

inline glm::vec3 ExtractVec3(const json& value, glm::vec3 def)
{
    if (value.is_array() && value.size() == 3) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec3(v.data());
    }

    return def;
}

inline glm::vec4 ExtractVec4(const json& value, glm::vec4 def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec4(v.data());
    }

    return def;
}

inline glm::quat ExtractQuat(const json& value, glm::quat def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::quat(v[3], v[0], v[1], v[2]);
    }

    return def;
}

} // namespace Toon

#endif // TOON_JSON_HPP