#include "StaticMeshComponent.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <glTF2.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Utils.hpp>


StaticMeshComponent::StaticMeshComponent(std::shared_ptr<Mesh> mesh)
{
    _mMeshes.push_back(mesh);

    // This is a current default for sceneloading the actual shader should be in the glTF file later
    _mShader = App::Inst()->GetShader("defaultLighting");
}

StaticMeshComponent::StaticMeshComponent(std::vector<std::shared_ptr<Mesh>> meshes)
{
    _mMeshes = meshes;

    // This is a current default for sceneloading the actual shader should be in the glTF file later
    _mShader = App::Inst()->GetShader("defaultLighting");
}

void StaticMeshComponent::Render()
{
    for (auto& mesh : _mMeshes)
    {
        mesh->Render(_mShader, GetEntity()->GetWorldTransform());
    }
}

bool StaticMeshComponent::Load(const std::string& filename)
{
    auto meshPrims = glTF2::LoadPrimitivesFromFile(filename);

    if (meshPrims.empty())
    {
        return false;
    }

    _mMeshes.push_back(std::make_unique<Mesh>(std::move(meshPrims)));

    return true;
}