#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>
#include <Component.hpp>

#include <vector>
#include <memory>

class Shader;

class MeshComponent : public Component
{
public:
    MeshComponent() = default;
    MeshComponent(std::shared_ptr<Mesh> mesh);
    MeshComponent(std::vector<std::shared_ptr<Mesh>> meshes);

    virtual ~MeshComponent() = default;

    void Render() override;

    void AddMesh(std::shared_ptr<Mesh> mesh) {
        _mMeshes.push_back(mesh);
    }

    void SetShader(Shader* value) { _mShader = value; }

    bool Load(const std::string& filename);

private:

    // Meshes
    std::vector<std::shared_ptr<Mesh>> _mMeshes;

    // Shader
    Shader* _mShader = nullptr;

};

#endif //MESH_COMPONENT_HPP