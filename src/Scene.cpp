#include "Scene.h"

Scene::Scene()
{

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

}