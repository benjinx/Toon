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

	float _mAngle = 0;
};

#endif // GAMESCENE_H