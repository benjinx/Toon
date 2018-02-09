#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"
#include "Shader.h"
#include "Utils.h"

class Scene : public GameObject
{
public:
	Scene();
	~Scene() {};
	
	virtual void Update(float dt);
	virtual void Render();
	void Pause();
	void Resume();

	std::unordered_map<std::string, GameObject*> GetGameObjects() { return _mGameObjects; }

protected:
	std::unordered_map<std::string, GameObject*> _mGameObjects;
};

#endif // SCENE_H