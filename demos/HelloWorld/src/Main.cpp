#include "Application.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "GameScene.hpp"

int main()
{
    Application app;
	Scene* gameScene = new GameScene();

    try
    {
		app.SetDemoName("Sandbox");
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
