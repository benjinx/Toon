#include "Temporality/Scene.hpp"

//#include <Temporality/App.hpp>
#include <Temporality/Camera.hpp>
#include <Temporality/DevUI.hpp>
//#include <Temporality/glTF2.hpp>
#include <Temporality/Log.hpp>
//#include <Temporality/Material.hpp>
//#include <Temporality/StaticMeshComponent.hpp>
//#include <Temporality/Shader.hpp>
#include <Temporality/Utils.hpp>

//#include <imgui/imgui.h>

namespace Temporality {

void Scene::Update(UpdateContext * ctx)
{
    /*Entity::Update(dt);

    // Use our default shaders and set the color & light position.
    App* app = App::Inst();
    Shader* defaultLighting = app->GetShader("defaultLighting");

    defaultLighting->Use();
    glm::vec3 defaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultLighting->SetVec3("lightColor", defaultLightColor);

    glm::vec4 defaultLightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    defaultLighting->SetVec4("lightPosition", defaultLightPosition);*/
}

void Scene::Render(RenderContext * ctx)
{
    Entity::Render(ctx);

    if (_mSkybox != nullptr)
    {
        _mSkybox->Render(ctx);
    }

    /*if (DevUI::showAxis)
    {
        RenderAxis();

        // Render the childrens
        for (auto& entity : _mChildren)
        {
            RenderAxis();
        }
    }*/
}

bool Scene::LoadScene(std::string filename)
{
    /*std::vector<std::unique_ptr<Entity>> loadedGobjs = glTF2::LoadSceneFromFile(filename);

    for (unsigned int i = 0; i < loadedGobjs.size(); ++i)
    {
        AddEntity(std::move(loadedGobjs[i]));
    }

    if (loadedGobjs.empty())
    {
        return false;
    }
    */
    return true;
}

void Scene::CreateSkybox(std::vector<std::string> faces)
{
    _mSkybox = std::make_unique<Skybox>();

    //_mSkybox->LoadCubemap(faces);
}

void Scene::Options()
{
    //ImGui::Checkbox("Show Entity Axis", &DevUI::showAxis);
}

} // namespace Temporality