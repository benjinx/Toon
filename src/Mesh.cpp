#include "Mesh.hpp"

#include <App.hpp>
#include <Material.hpp>
#include <Shader.hpp>

Mesh::Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material)
{
    _mPrimitives.push_back(Primitive { vao, mode, count, type, offset, material });
}

Mesh::Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material, Joint rootJoint, int jointCount)
{
    _mPrimitives.push_back(Primitive { vao, mode, count, type, offset, material });
    _mRootJoint = rootJoint;
    _mJointCount = jointCount;
    _mRootJoint.CalcInverseBindTransform(glm::mat4());
}

Mesh::Mesh(std::vector<Primitive>&& primitives)
{
    LoadFromData(std::move(primitives));
}

bool Mesh::LoadFromData(std::vector<Primitive>&& primitives)
{
    for (auto&& p : primitives) {
        _mPrimitives.push_back(std::move(p));
    }

    return true;
}

void Mesh::Update(const float dt)
{
    _mAnimator.Update(dt);
}

std::vector<glm::mat4> Mesh::GetJointTransforms()
{
    // Create a new vector 
    // vector needs the size of _mJointCount
    // we then pass the vector and root joint to AddJointsToArray

    std::vector<glm::mat4> jointMatrices;
    AddJointsToArray(_mRootJoint, jointMatrices);
    return jointMatrices;
}

void Mesh::AddJointsToArray(Joint headJoint, std::vector<glm::mat4> jointMatrices)
{
    // we have the transforms and we need to stick them in the corrosponding indexes.
    // video #2 7:33

    jointMatrices[headJoint.GetIndex()] = headJoint.GetTransform();

    for (Joint& childJoint : headJoint.GetChildren())
    {
        AddJointsToArray(childJoint, jointMatrices);
    }

    return;
}

void Mesh::Render(Shader * shader, glm::mat4 modelMat)
{
    if (!shader || !App::Inst()->GetCurrentCamera()) {
        return;
    }

    shader->Use();

    const auto& view = App::Inst()->GetCurrentCamera()->GetView();
    const auto& proj = App::Inst()->GetCurrentCamera()->GetProjection();

    shader->SetMat4("modelMat", modelMat);
    shader->SetMat4("viewMat", view);
    shader->SetMat4("projMat", proj);

    glm::mat4 mvp = proj * view * modelMat;
    shader->SetMat4("mvp", mvp);

    for (const auto& p : _mPrimitives)
    {
        if (p._Material != nullptr)
        {
            p._Material->Bind(shader);
        }

        glBindVertexArray(p.VAO);
        //glDrawArrays(p.Mode, 0, p.Count);
        glDrawElements(p.Mode, p.Count, p.Type, (char*)0 + p.Offset);
        glBindVertexArray(0);
    }
}