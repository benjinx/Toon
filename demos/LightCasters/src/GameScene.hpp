#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

class GameScene : public Scene
{
public:
	GameScene() {};

	void Start() override;
	void Update(float dt) override;

private:
	bool _mDirLight, _mPointLight, _mSpotLight;
};

#endif // GAMESCENE_H