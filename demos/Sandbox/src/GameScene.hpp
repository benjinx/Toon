#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Light.hpp"

class GameScene : public Scene
{
public:
	GameScene() {};
	~GameScene() { DeleteShaders(); };

	void Start() override;
	void Update(float dt) override;
	void SetupShaders() override;
	void DeleteShaders() override;
	void PhysicsStart();
	void PhysicsUpdate(float dt);

private:
	bool _mDirLight, _mPointLight, _mSpotLight;
};

#endif // GAMESCENE_H