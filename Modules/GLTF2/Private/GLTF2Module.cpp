#include <Toon/Module.hpp>

#include <Toon/GLTF2/GLTF2MeshImporter.hpp>
#include <Toon/GLTF2/GLTF2SceneImporter.hpp>

namespace Toon::GLTF2 {

bool ModuleInit() {

    return true;
}

void ModuleTerm() {
}

TOON_MODULE {
    .Name       = "ToonGLTF2",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::GLTF2