#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"
#include "Window.h"
#include "Camera.h"

class GameScene : Scene
{
public:
	GameScene() {};
	~GameScene() { _mShader.Destroy(); };

	void Start();
	void Update(float dt) override;
	void Render() override;
	void SetupShaders();
	void DeleteShaders();
	void PhysicsStart();
	void PhysicsUpdate(float dt);

private:
	Shader _mShader;
	int _mNumShaders;

	std::vector<Shader> _mShaders;
};

#endif // GAMESCENE_H