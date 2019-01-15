#ifndef SCENE_H
#define SCENE_H

#include <Axis.hpp>
#include <Config.hpp>
#include <GameObject.hpp>

#include <unordered_map>

class Scene //: public GameObject
{
public:

	typedef std::unordered_map<std::string, GameObject*> GameObjectMap;

	Scene() = default;
	virtual ~Scene();
	
	virtual void Start();
	virtual void Pause() { }
	virtual void Resume() { }

	virtual void Update(float dt);
	virtual void Render();
	
	virtual void SetupShaders() { }

	GameObjectMap GetGameObjects() { return _mGameObjects; }

	// Ui Options.
	static void Options();

protected:
	// Show gameobject axis?
	static bool _sShowAxis;

	GameObjectMap _mGameObjects;

	Axis* _mSceneAxis;
};

#endif // SCENE_H