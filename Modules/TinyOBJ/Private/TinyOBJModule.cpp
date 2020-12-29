#include <Temporality/Module.hpp>
#include <Temporality/TinyOBJ/TinyOBJMeshImporter.hpp>

namespace Temporality::TinyOBJ {

bool ModuleInit() {
    AddMeshImporter("TinyOBJ", std::make_unique<TinyOBJMeshImporter>());
    
    return true;
}

void ModuleTerm() {
    RemoveMeshImporter("TinyOBJ");
}

TEMPORALITY_MODULE {
    .Name       = "TinyOBJ",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Temporality::TinyOBJ