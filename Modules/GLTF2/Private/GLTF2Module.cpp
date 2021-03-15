#include <Toon/Module.hpp>

#include <Toon/GLTF2/GLTF2MeshImporter.hpp>
#include <Toon/GLTF2/GLTF2SceneImporter.hpp>

namespace Toon::GLTF2 {

bool ModuleInit() {
    AddMeshImporter("GLTF2", std::unique_ptr<MeshImporter>(new GLTF2MeshImporter()));
    AddSceneImporter("GLTF2", std::unique_ptr<SceneImporter>(new GLTF2SceneImporter()));

    return true;
}

void ModuleTerm() {
    RemoveSceneImporter("GLTF2");
    RemoveMeshImporter("GLTF2");
}

TOON_MODULE {
    .Name       = "ToonGLTF2",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::GLTF2