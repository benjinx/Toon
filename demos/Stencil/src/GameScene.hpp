#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

class GameScene : public Scene
{
public:
    GameScene() {};

    void Start() override;
    void Update(float dt) override;
    void Render() override;

private:
    GameObject* _mPlane;
    GameObject* _mSphere;
    GameObject* _mCube;
    GameObject* _mTorus;
    MeshComponent* _mPlaneMesh;
    MeshComponent* _mSphereMesh;
    MeshComponent* _mCubeMesh;
    MeshComponent* _mTorusMesh;
};

#endif // GAMESCENE_H