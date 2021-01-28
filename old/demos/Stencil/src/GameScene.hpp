#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Toon.hpp>

class GameScene : public Scene
{
public:
    GameScene() {};

    void Start() override;
    void Update(float dt) override;
    void Render() override;

private:
    Entity* _mPlane;
    Entity* _mSphere;
    Entity* _mCube;
    Entity* _mTorus;
    StaticMeshComponent* _mPlaneMesh;
    StaticMeshComponent* _mSphereMesh;
    StaticMeshComponent* _mCubeMesh;
    StaticMeshComponent* _mTorusMesh;
};

#endif // GAMESCENE_H