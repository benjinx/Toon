#include "Scene.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

bool Scene::_sShowAxis = false;

Scene::~Scene() {
    for (auto& go : _mGameObjects) {
        delete go.second;
    }
    _mGameObjects.clear();
}

void Scene::Render()
{
	for (auto& gameObject : _mGameObjects)
	{
		if (gameObject.first == "Sun" || gameObject.first == "Light")
			gameObject.second->Render(_mShaders[1]);
		else
			gameObject.second->Render(_mShaders[2]);
	}

	if (_sShowAxis)
	{
		for (auto& gameObject : _mGameObjects)
		{
			gameObject.second->DrawAxis(_mShaders[0]);
		}
	}
}

void Scene::Options()
{
	ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}