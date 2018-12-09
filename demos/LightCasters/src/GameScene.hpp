#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

class GameScene : public Scene
{
public:
	GameScene() {};
	~GameScene() { DeleteShaders(); };

	void Start() override;
	void Update(float dt) override;
	void SetupShaders() override;
	void DeleteShaders() override;

private:

	bool _mOptions = true, _mDirLight = true, _mPointLight = true, _mSpotLight = true;
};

#endif // GAMESCENE_H