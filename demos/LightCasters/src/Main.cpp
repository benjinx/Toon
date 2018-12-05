#include "Application.hpp"
#include "GameScene.hpp"

int main()
{
	const auto& paths = Utils::GetResourcePaths();

    Application app;
	Scene* gameScene = new GameScene();

    try
    {
		app.SetDemoName("LightCasters");
		app.SetCurrentScene(gameScene);
        app.Run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
