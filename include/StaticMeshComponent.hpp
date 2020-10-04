#ifndef STATIC_MESH_COMPONENT_HPP
#define STATIC_MESH_COMPONENT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>
#include <Component.hpp>

#include <vector>
#include <memory>

class Shader;

class StaticMeshComponent : public Component
{
public:
    StaticMeshComponent() = default;
    StaticMeshComponent(std::shared_ptr<Mesh> mesh);
    StaticMeshComponent(std::vector<std::shared_ptr<Mesh>> meshes);

    virtual ~StaticMeshComponent() = default;

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

#endif // STATIC_MESH_COMPONENT_HPP