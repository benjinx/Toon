#include "Toon/Scene.hpp"

//#include <Toon/App.hpp>
#include <Toon/Camera.hpp>
#include <Toon/DevUI.hpp>
//#include <Toon/glTF2.hpp>
#include <Toon/Log.hpp>
//#include <Toon/Material.hpp>
//#include <Toon/StaticMeshComponent.hpp>
//#include <Toon/Shader.hpp>
#include <Toon/Utils.hpp>

//#include <imgui/imgui.h>

namespace Toon {

TOON_ENGINE_API
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

TOON_ENGINE_API
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

TOON_ENGINE_API
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

TOON_ENGINE_API
void Scene::CreateSkybox(std::vector<std::string> faces)
{
    _mSkybox = std::make_unique<Skybox>();

    //_mSkybox->LoadCubemap(faces);
}

TOON_ENGINE_API
void Scene::Options()
{
    //ImGui::Checkbox("Show Entity Axis", &DevUI::showAxis);
}

} // namespace Toon