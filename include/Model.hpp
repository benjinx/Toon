#ifndef MODEL_HPP
#define MODEL_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>

#include <vector>
#include <memory>

enum AttributeID : GLint
{
    POSITION = 0,
    NORMAL = 1,
    TEXCOORD = 2,
    TANGENT = 3,
    BITANGENT = 4,
    COLOR = 5,
};

class Shader;

class Model
{
public:
    
    Model(std::vector<std::unique_ptr<Mesh>> && meshes);

    virtual ~Model() = default;

    void Render(Shader* shader, glm::mat4 modelMatrix);

    void AddMesh(std::unique_ptr<Mesh> && mesh) { 
        _mMeshes.push_back(std::move(mesh));
    }

private:

    // Mesh
    std::vector<std::unique_ptr<Mesh>> _mMeshes;

};

#endif //MODEL_HPP