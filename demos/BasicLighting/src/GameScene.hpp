#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

class GameScene : public Scene
{
public:
	GameScene() {};

	void Start() override;
	void Update(float dt) override;
};

#endif // GAMESCENE_H