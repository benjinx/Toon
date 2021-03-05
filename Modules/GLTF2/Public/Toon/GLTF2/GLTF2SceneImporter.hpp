#ifndef TOON_GLTF2_SCENE_IMPORTER_HPP
#define TOON_GLTF2_SCENE_IMPORTER_HPP

#include <Toon/GLTF2/GLTF2Config.hpp>

#include <Toon/SceneImporter.hpp>

namespace Toon::GLTF2 {

class TOON_GLTF2_API GLTF2SceneImporter : public SceneImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2SceneImporter)

    GLTF2SceneImporter() = default;

    virtual bool LoadFromFile(Entity * root, const string& filename) override;

}; // class GLTF2SceneImporter

} // namespace Toon::GLTF2

#endif // TOON_GLTF2_SCENE_IMPORTER_HPP