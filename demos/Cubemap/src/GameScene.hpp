#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

enum class Mode : int
{
    REFLECTION,
    REFRACTION,
};

class GameScene : public Scene
{
public:
    GameScene() {};

    void Start() override;
    void Update(float dt) override;
    void Render() override;

private:
    Mode _mMode = Mode::REFLECTION;
};

#endif // GAMESCENE_H