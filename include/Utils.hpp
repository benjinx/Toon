#ifndef UTILITIES_H
#define UTILITIES_H

#include <Config.hpp>
#include <Math.hpp>

#include <sstream>
#include <vector>

class Mesh;

namespace Utils
{
    void SetAssetPath(const std::string& path);
    std::string GetAssetPath();
    std::vector<std::string> GetResourcePaths();
    void CleanSlashes(std::string& path);
    std::string GetBasename(std::string path);
    std::string GetDirname(std::string path);
    std::string GetExtension(std::string path);

    //Mesh* Get2DMesh(glm::vec4 screenCords, glm::vec4 textureCords);

    inline glm::vec3 GetWorldUp() { return glm::vec3(0.0f, 1.0f, 0.0f); }
    inline glm::vec3 GetWorldForward() { return glm::vec3(0.0f, 0.0f, -1.0f); }
}

#endif // UTILITIES_H
