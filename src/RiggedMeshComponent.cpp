#include "RiggedMeshComponent.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <glTF2.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Utils.hpp>


RiggedMeshComponent::RiggedMeshComponent(std::shared_ptr<Mesh> mesh)
{
    _mMeshes.push_back(mesh);

    // This is a current default for sceneloading the actual shader should be in the glTF file later
    _mShader = App::Inst()->GetShader("defaultLighting");
}

RiggedMeshComponent::RiggedMeshComponent(std::vector<std::shared_ptr<Mesh>> meshes)
{
    _mMeshes = meshes;

    // This is a current default for sceneloading the actual shader should be in the glTF file later
    _mShader = App::Inst()->GetShader("defaultLighting");
}

void RiggedMeshComponent::Update(const float dt)
{
    _mAnimator.Update(dt);
}

void RiggedMeshComponent::Render()
{
    for (auto& mesh : _mMeshes)
    {
        mesh->Render(_mShader, GetGameObject()->GetWorldTransform());
    }
}

bool RiggedMeshComponent::Load(const std::string& filename)
{
    auto meshPrims = glTF2::LoadPrimitivesFromFile(filename);

    if (meshPrims.empty())
    {
        return false;
    }

    _mMeshes.push_back(std::make_unique<Mesh>(std::move(meshPrims)));

    return true;
}

glm::mat4 RiggedMeshComponent::GetJointTransforms()
{
    //glm::mat4 jointMatrices[] = { glm::mat4() };
    //addJointsToArray(_mRootJoint, jointMatrices);
}

void RiggedMeshComponent::AddJointsToArray()
{

}