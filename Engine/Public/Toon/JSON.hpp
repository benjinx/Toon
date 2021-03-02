#ifndef TOON_JSON_HPP
#define TOON_JSON_HPP

#include <Toon/Config.hpp>
#include <Toon/String.hpp>

#define JSON_NOEXCEPTION
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

} // namespace Toon

#endif // TOON_JSON_HPP