#ifndef RIGGED_MESH_COMPONENT_HPP
#define RIGGED_MESH_COMPONENT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>
#include <Component.hpp>

#include <vector>
#include <memory>

class Shader;

class RiggedMeshComponent : public Component
{
public:
    RiggedMeshComponent() = default;
    RiggedMeshComponent(std::shared_ptr<Mesh> mesh);
    RiggedMeshComponent(std::vector<std::shared_ptr<Mesh>> meshes);

    virtual ~RiggedMeshComponent() = default;

    void Update(const float dt) override;
    void Render() override;

    void AddMesh(std::shared_ptr<Mesh> mesh) {
        _mMeshes.push_back(mesh);
    }

    bool Load(const std::string& filename);

    void SetShader(Shader* value) { _mShader = value; }

private:

    // Meshes
    std::vector<std::shared_ptr<Mesh>> _mMeshes;

    // Shader
    Shader* _mShader = nullptr;

};

#endif // RIGGED_MESH_COMPONENT_HPP