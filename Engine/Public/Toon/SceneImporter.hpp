#ifndef TOON_SCENE_IMPORTER_HPP
#define TOON_SCENE_IMPORTER_HPP

#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>

#include <Toon/String.hpp>
#include <memory>
#include <vector>

namespace Toon {

class TOON_ENGINE_API SceneImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(SceneImporter)

    SceneImporter() = default;

    virtual ~SceneImporter() = default;

    virtual bool LoadFromFile(Entity * root, const string& filename) = 0;

}; // class SceneImporter

TOON_ENGINE_API
void AddSceneImporter(const string& id, std::unique_ptr<SceneImporter> importer);

TOON_ENGINE_API
void RemoveSceneImporter(const string& id);

TOON_ENGINE_API
const std::vector<SceneImporter *>& GetAllSceneImporters();

} // namespace Toon

#endif // TOON_SCENE_IMPORTER_HPP
