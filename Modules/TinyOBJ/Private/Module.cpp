#include <Temporality/Module.hpp>
#include <Temporality/TinyOBJ/MeshImporter.hpp>

namespace Temporality::TinyOBJ {

void ModuleInit() {
    AddMeshImporter("TinyOBJ", std::make_unique<MeshImporter>());
}

void ModuleTerm() {
    RemoveMeshImporter("TinyOBJ");
}

TEMPORALITY_MODULE() {
    "TinyOBJ",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::TinyOBJ