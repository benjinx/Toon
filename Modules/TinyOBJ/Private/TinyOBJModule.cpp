#include <Toon/Module.hpp>
#include <Toon/TinyOBJ/TinyOBJMeshImporter.hpp>

namespace Toon::TinyOBJ {

bool ModuleInit() {
    AddMeshImporter("TinyOBJ", std::unique_ptr<TinyOBJMeshImporter>(new TinyOBJMeshImporter));
    
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