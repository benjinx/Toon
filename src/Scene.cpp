#include "Scene.hpp"

#include "Application.hpp"
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
		gameObject.second->Render();
	}

	if (_sShowAxis)
	{
		for (auto& gameObject : _mGameObjects)
		{
			gameObject.second->DrawAxis();
		}
	}
}

void Scene::Options()
{
	ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}