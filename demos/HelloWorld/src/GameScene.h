#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"
#include "Window.h"
#include "Camera.h"
#include "ScriptHost.h"

class GameScene : public Scene
{
public:
	GameScene() {};
	~GameScene() { _mShader.Destroy(); };

	void Start() override;
	void Update(float dt) override;
	void Render() override;
	void SetupShaders() override;
	void DeleteShaders() override;
	void PhysicsStart();
	void PhysicsUpdate(float dt);

private:
	Shader _mShader;
	int _mNumShaders;
	int _mProg;
	ScriptHost _mScriptHost;

	std::vector<Shader> _mShaders;
};

#endif // GAMESCENE_H