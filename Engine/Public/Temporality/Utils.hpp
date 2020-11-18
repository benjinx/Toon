#ifndef UTILS_HPP
#define UTILS_HPP

#include <Temporality/Config.hpp>

#include <string>

namespace Temporality {

inline void CleanSlashes(std::string& path)
{
    for (unsigned int i = 0; i < path.size(); ++i)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }
}

inline std::string GetBasename(std::string path)
{
    CleanSlashes(path);
        size_t pivot = path.find_last_of('/');
        return (pivot == std::string::npos
            ? std::string()
            : path.substr(pivot + 1));
}

}

#endif // UTILS_HPP