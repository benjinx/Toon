#ifndef UTILS_HPP
#define UTILS_HPP

#include <Toon/Config.hpp>

#include <string>
#include <vector>

namespace Toon {

inline std::string GetDirname(std::string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == std::string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == std::string::npos
        ? path
        : path.substr(0, pivot)
    );
}

inline std::string GetBasename(std::string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == std::string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == std::string::npos
        ? path
        : path.substr(pivot + 1)
    );
}

inline std::string GetExtension(std::string path)
{
    size_t pivot = path.find_last_of('.');
    return (
        pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1)
    );
}

TOON_ENGINE_API
std::vector<std::string> GetAssetPaths();

} // namespace Toon

#endif // UTILS_HPP