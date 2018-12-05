#include "Application.hpp"
#include "GameScene.hpp"

int main()
{
	const auto& paths = Utils::GetResourcePaths();
	for (auto& p : paths) {
		printf("%s\n", p.c_str());
	}

    Application app;
	Scene* gameScene = new GameScene();

    try
    {
		app.SetDemoName("AdvancedLighting");
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