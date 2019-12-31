#include "Mesh.hpp"

#include <App.hpp>
#include <Material.hpp>
#include <Shader.hpp>

Mesh::Mesh(GLuint vao, GLenum mode, GLsizei count, std::shared_ptr<Material> material)
    : _mVAO(vao)
    , _mMode(mode)
    , _mCount(count)
    , _mMaterial(material)
{ }

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

    if (_mMaterial != nullptr)
    {
        _mMaterial->Bind(shader);
    }

    glBindVertexArray(_mVAO);
    glDrawArrays(_mMode, 0, _mCount);
    glBindVertexArray(0);
}