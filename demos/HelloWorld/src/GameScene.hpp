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
	void Render() override;
	void SetupShaders() override;
	void DeleteShaders() override;
	void PhysicsStart();
	void PhysicsUpdate(float dt);

private:
	int _mNumShaders;
	int _mProg;

	std::vector<Shader*> _mShaders;
};

#endif // GAMESCENE_H