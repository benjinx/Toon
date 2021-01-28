#include <Toon.hpp>

#include "GameScene.hpp"

int main(int argc, char * argv[])
{
    Utils::SetAssetPath(RESOURCE_PATH);
    const auto& paths = Utils::GetResourcePaths();
    for (auto& p : paths) {
        ToonLogInfo("%s", p);
    }

    App app;
    Scene* gameScene = new GameScene();

    app.SetCurrentScene(gameScene);

    if (!app.Start())
    {
        return EXIT_FAILURE;
    }

    app.Run();

    delete gameScene;

    return EXIT_SUCCESS;
}
