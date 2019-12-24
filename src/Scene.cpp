#include "Scene.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Utils.hpp>

#include <imgui/imgui.h>

#include <nlohmann/json.hpp>

#include <stb/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

bool Scene::_sShowAxis = false;

void Scene::Update(float dt)
{
    GameObject::Update(dt);

    // Use our default shaders and set the color & light position.
    App* app = App::Inst();
    Shader* defaultLighting = app->GetShader("defaultLighting");

    defaultLighting->Use();
    glm::vec3 defaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultLighting->SetVec3("lightColor", defaultLightColor);

    glm::vec4 defaultLightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    defaultLighting->SetVec4("lightPosition", defaultLightPosition);
}

void Scene::Render()
{
    GameObject::Render();

    if (_sShowAxis)
    {
        for (auto& gameObject : _mChildren)
        {
            RenderAxis();
        }
    }
}

bool Scene::Load(std::string filename)
{
    bool loaded = GameObject::Load(filename);

    if (loaded)
        return true;
    
    return false;
}

GameObject* Scene::AddGameObject(std::string name, std::unique_ptr<GameObject> gameObject)
{
    gameObject->SetName(name);
    _mChildren.push_back(std::move(gameObject));
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject()
{
    _mChildren.push_back(std::make_unique<GameObject>());
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject(std::string name)
{
    _mChildren.push_back(std::make_unique<GameObject>());
    _mChildren.back()->SetName(name);
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject(std::unique_ptr<GameObject> gobj)
{
    _mChildren.push_back(std::move(gobj));
    return _mChildren.back().get();
}

void Scene::Options()
{
    ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}