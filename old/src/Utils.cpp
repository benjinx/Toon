#include "Utils.hpp"

#include <Log.hpp>

#include <algorithm>
#include <sstream>

namespace Utils
{
    std::string _mAssetPath;

    std::vector<std::string> _mAssetPaths;


    void SetAssetPath(const std::string& path)
    {
        //LogInfo("Setting Asset Path: %s\n", path.c_str());
        _mAssetPath = path;
        _mAssetPaths.clear();
    }

    std::string GetAssetPath()
    {
        return _mAssetPath;
    }

    std::vector<std::string> GetResourcePaths()
    {
        if (_mAssetPaths.empty()) {
            std::stringstream ss(GetAssetPath());
            std::string path;
            while (std::getline(ss, path, ':')) {
                if (path.empty()) continue;
                if (path.back() != '/') path.push_back('/');

                _mAssetPaths.push_back(path);
            }
            _mAssetPaths.push_back("");
            std::reverse(_mAssetPaths.begin(), _mAssetPaths.end());
        }
        return _mAssetPaths;
    }

    void CleanSlashes(std::string& path)
    {
        for (unsigned int i = 0; i < path.size(); ++i)
        {
            if (path[i] == '\\')
            {
                path[i] = '/';
            }
        }
    }

    std::string GetBasename(std::string path)
    {
        CleanSlashes(path);
        size_t pivot = path.find_last_of('/');
        return (pivot == std::string::npos
            ? std::string()
            : path.substr(pivot + 1));
    }

    std::string GetDirname(std::string path)
    {
        CleanSlashes(path);
        size_t pivot = path.find_last_of('/');
        return (pivot == std::string::npos
            ? "./"
            : path.substr(0, pivot));
    }

    std::string GetExtension(std::string path)
    {
        size_t pivot = path.find_last_of('.');
        return (pivot == std::string::npos
            ? std::string()
            : path.substr(pivot + 1));
    }

    //Mesh* Get2DMesh(glm::vec4 screenCords, glm::vec4 textureCords)
    //{
    //    GLuint vbos[2];
    //    GLuint vao;
    //    std::vector<glm::vec3> vertices = {
    //        { screenCords[2], screenCords[1], 0 },
    //    { screenCords[2], screenCords[3], 0 },
    //    { screenCords[0], screenCords[3], 0 },
    //    { screenCords[2], screenCords[1], 0 },
    //    { screenCords[0], screenCords[3], 0 },
    //    { screenCords[0], screenCords[1], 0 } };

    //    std::vector<glm::vec2> texCoords = {
    //        { textureCords[2], textureCords[3] },
    //    { textureCords[2], textureCords[1] },
    //    { textureCords[0], textureCords[1] },
    //    { textureCords[2], textureCords[3] },
    //    { textureCords[0], textureCords[1] },
    //    { textureCords[0], textureCords[3] } };

    //    glGenVertexArrays(1, &vao);
    //    glBindVertexArray(vao);

    //    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
    //    glVertexAttribPointer(Mesh::AttributeID::POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //    glEnableVertexAttribArray(Mesh::AttributeID::POSITION);

    //    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, texCoords.data(), GL_STATIC_DRAW);
    //    glVertexAttribPointer(Mesh::AttributeID::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    //    glEnableVertexAttribArray(Mesh::AttributeID::TEXCOORD);

    //    glBindVertexArray(0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //    Mesh* m = new Mesh(vao, 0, 0, {});

    //    return m;
    //    // std::vector<glm::vec3> Vertices, {}, std::vector<glm::vec2> texCoords, {}, {}
    //    /*
    //    return new Mesh(
    //    {
    //    {screenCords[2], screenCords[1], 0},
    //    {screenCords[2], screenCords[3], 0},
    //    {screenCords[0], screenCords[3], 0},
    //    {screenCords[2], screenCords[1], 0},
    //    {screenCords[0], screenCords[3], 0},
    //    {screenCords[0], screenCords[1], 0} },
    //    {},
    //    {
    //    {textureCords[2], textureCords[3]},
    //    {textureCords[2], textureCords[1]},
    //    {textureCords[0], textureCords[1]},
    //    {textureCords[2], textureCords[3]},
    //    {textureCords[0], textureCords[1]},
    //    {textureCords[0], textureCords[3]} },
    //    {}, {});
    //    */
    //}
}
