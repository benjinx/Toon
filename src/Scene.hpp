#ifndef SCENE_H
#define SCENE_H

#include "GameObject.hpp"
#include "Shader.hpp"
#include "Utils.hpp"

class Scene : public GameObject
{
public:
	Scene();
	~Scene() {};
	
	virtual void Start() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void SetupShaders() = 0;
	virtual void DeleteShaders() = 0;
	void Pause() {};
	void Resume() {};

	std::unordered_map<std::string, GameObject*> GetGameObjects() { return _mGameObjects; }

protected:
	std::unordered_map<std::string, GameObject*> _mGameObjects;
};

#endif // SCENE_H