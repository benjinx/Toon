#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Toon.hpp>

class GameScene : public Scene
{
public:
    GameScene() {};

    void Start() override;
    void Update(float dt) override;

private:

    float _mAngle = 0;
    float _mMarsAngle = 0;
};

#endif // GAMESCENE_H