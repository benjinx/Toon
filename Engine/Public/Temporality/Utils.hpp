#ifndef UTILS_HPP
#define UTILS_HPP

#include <Temporality/Config.hpp>

#include <string>
#include <vector>

namespace Temporality {

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

TEMPORALITY_ENGINE_API
std::vector<std::string> GetAssetPaths();

} // namespace Temporality

#endif // UTILS_HPP