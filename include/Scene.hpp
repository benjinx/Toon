#ifndef SCENE_H
#define SCENE_H

#include <Axis.hpp>
#include <Config.hpp>
#include <GameObject.hpp>
#include <Mesh.hpp>

#include <memory>
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

	bool Load(std::string filename);


	GameObjectMap GetGameObjects() { return _mGameObjects; }

	// Ui Options.
	static void Options();

protected:

	void AddGameObject(std::unique_ptr<GameObject> gameObject);

	static bool _sShowAxis;

	GameObjectMap _mGameObjects;

	Axis* _mSceneAxis;

	glm::mat4 _mSceneTransform = glm::mat4(1);

	std::vector<Mesh*> _mMeshes;
};

#endif // SCENE_H