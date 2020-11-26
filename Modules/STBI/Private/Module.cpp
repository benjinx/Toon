#include <Temporality/Module.hpp>

#include <Temporality/STBI/TextureImporter.hpp>
#include <Temporality/Log.hpp>

namespace Temporality::STBI {

void ModuleInit() {
    AddTextureImporter("STBI", std::unique_ptr<Temporality::TextureImporter>(new TextureImporter()));
    LogInfo("Init");
}

void ModuleTerm() {
    RemoveTextureImporter("STBI");
    LogInfo("Term()");
}

TEMPORALITY_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::STBI