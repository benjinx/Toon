#include "Toon/Scene.hpp"

namespace Toon {

static Toon::Scene * _CurrentScene = nullptr;

TOON_ENGINE_API
System * Scene::AddSystem(std::unique_ptr<System> && sys)
{
    _systemPtrs.push_back(sys.get());
    _systems.push_back(std::move(sys));

    return _systemPtrs.back();
}

TOON_ENGINE_API
std::vector<System *> Scene::GetSystems() const
{
    return _systemPtrs;
}

TOON_ENGINE_API
void SetCurrentScene(Scene * scene)
{
    _CurrentScene = scene;
}

TOON_ENGINE_API
Scene * GetCurrentScene()
{
    return _CurrentScene;
}

} // namespace Toon