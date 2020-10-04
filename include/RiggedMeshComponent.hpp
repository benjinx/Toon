#ifndef RIGGED_MESH_COMPONENT_HPP
#define RIGGED_MESH_COMPONENT_HPP

#include <Animator.hpp>
#include <Config.hpp>
#include <Joint.hpp>
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

    void doAnimation(Animation animation) { _mAnimator.doAnimation(animation); }

    void SetShader(Shader* value) { _mShader = value; }

    Joint GetRootJoint() { return _mRootJoint; }

    glm::mat4 GetJointTransforms();

    void AddJointsToArray();

private:

    // Meshes
    std::vector<std::shared_ptr<Mesh>> _mMeshes;

    // Skeleton/Animation
    Joint _mRootJoint;
    int _mJointCount;
    Animator _mAnimator;

    // Shader
    Shader* _mShader = nullptr;

};

#endif // RIGGED_MESH_COMPONENT_HPP