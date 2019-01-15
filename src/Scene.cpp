#include "Scene.hpp"

#include <imgui/imgui.h>

bool Scene::_sShowAxis = false;

Scene::~Scene() {
	delete _mSceneAxis;

    for (auto& go : _mGameObjects) {
        delete go.second;
    }
    _mGameObjects.clear();
}

void Scene::Start()
{
	_mSceneAxis = new Axis();
}

void Scene::Update(float dt)
{
	for (auto& gobj : _mGameObjects)
	{
		gobj.second->Update(dt);
	}
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
			//gameObject.second->DrawAxis();
			_mSceneAxis->Render(gameObject.second->GetWorldTransform());
		}
	}
}

void Scene::Options()
{
	ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}