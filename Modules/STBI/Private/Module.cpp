#include <Temporality/Module.hpp>

#include <Temporality/STBI/TextureImporter.hpp>
#include <Temporality/Log.hpp>

namespace Temporality::STBI {

void ModuleInit() {
    TextureImporter::Register("STBI", std::unique_ptr<Temporality::TextureImporter>(new TextureImporter()));
    LogInfo("STBI Init!");
}

void ModuleTerm() {
    TextureImporter::Unregister("STBI");
    LogInfo("STBI Term!");
}

TEMPORALITY_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::STBI