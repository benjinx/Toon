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

Scene::~Scene() {
    delete _mSceneAxis;
}

void Scene::Start()
{
    _mSceneAxis = new Axis();
}

void Scene::Update(float dt)
{
    // Use default shader
    App* app = App::Inst();
    Shader* defaultLighting = app->GetShader("defaultLighting");

    defaultLighting->Use();
    glm::vec3 defaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultLighting->SetVec3("lightColor", defaultLightColor);

    glm::vec4 defaultLightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    defaultLighting->SetVec4("lightPosition", defaultLightPosition);

    //for (auto& gobj : _mGameObjects)
    //{
    //    // Update gobjs
    //    gobj.second->Update(dt);
    //}

    for (auto& gobj : GetChildren())
    {
        // Need to also update myself?
        gobj->Update(dt);
    }
}

void Scene::Render()
{
    //for (auto& gameObject : _mGameObjects)
    //{
    //    // Render gobjs
    //    gameObject.second->Render();
    //}

    for (auto& gobj : GetChildren())
    {
        gobj->Render();
    }

    if (_sShowAxis)
    {
        for (auto& gameObject : GetChildren())
        {
            // Render the gobjs axis
            _mSceneAxis->Render(gameObject->GetWorldTransform());

            // Render the scenes axis
            _mSceneAxis->Render(_mSceneTransform);
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

void Scene::AddGameObject(std::string name, GameObject* gameObject)
{
    if (gameObject)
    {
        gameObject->SetName(name);
        AddChild(gameObject);
    }
}

GameObject* Scene::AddGameObject()
{
    GameObject* tmp = new GameObject();
    tmp->SetName("tmp" + rand());
    AddChild(tmp);
    return GetGameObject(tmp->GetName());
}

GameObject* Scene::AddGameObject(std::string name)
{
    GameObject* tmp = new GameObject();
    tmp->SetName(name);
    AddChild(tmp);
    return GetGameObject(name);
}

void Scene::AddGameObject(GameObject* gobj)
{
    AddChild(gobj);
}

GameObject* Scene::GetGameObject(std::string name)
{
    for (auto gobj : GetChildren())
    {
        if (gobj->GetName() == name)
        {
            return gobj;
        }
    }

    return new GameObject();
}

void Scene::Options()
{
    ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}