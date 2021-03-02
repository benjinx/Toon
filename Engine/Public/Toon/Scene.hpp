#ifndef TOON_SCENE_H
#define TOON_SCENE_H

#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>
#include <Toon/System.hpp>

#include <memory>
#include <vector>

namespace Toon {

class TOON_ENGINE_API Scene : public Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene)

    Scene() = default;

    virtual ~Scene() = default;
    
    System * AddSystem(std::unique_ptr<System> && sys);

    std::vector<System *> GetSystems() const;

private:
    std::vector<std::unique_ptr<System>> _systems;

    std::vector<System *> _systemPtrs;

}; // class Scene

TOON_ENGINE_API
void SetCurrentScene(Scene * scene);

TOON_ENGINE_API
Scene * GetCurrentScene();

} // namespace Toon

#endif // TOON_SCENE_H