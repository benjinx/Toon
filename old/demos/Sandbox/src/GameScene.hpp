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
    void LogTest();

private:
    StaticMeshComponent* _mHelmMesh;
    StaticMeshComponent* _mCubeMesh;
    GameObject* _mHelm;
    GameObject* _mCube;
};

#endif // GAMESCENE_H