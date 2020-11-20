#include <Temporality/Module.hpp>

#include <Temporality/STBI/TextureImporter.hpp>

namespace Temporality::STBI {

void ModuleInit() {
    TextureImporter::Register("STBI", std::unique_ptr<Temporality::TextureImporter>(new TextureImporter()));
}

void ModuleTerm() {
    TextureImporter::Unregister("STBI");
}

TEMPORALITY_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::STBI