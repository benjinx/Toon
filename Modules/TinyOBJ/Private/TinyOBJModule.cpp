#include <Toon/Module.hpp>
#include <Toon/TinyOBJ/TinyOBJMeshImporter.hpp>

namespace Toon::TinyOBJ {

bool ModuleInit() {
    AddMeshImporter("TinyOBJ", std::make_unique<TinyOBJMeshImporter>());
    
    return true;
}

void ModuleTerm() {
    RemoveMeshImporter("TinyOBJ");
}

TOON_MODULE {
    .Name       = "TinyOBJ",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::TinyOBJ