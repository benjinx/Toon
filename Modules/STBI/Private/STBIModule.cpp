#include <Toon/Module.hpp>

#include <Toon/STBI/STBITextureImporter.hpp>

namespace Toon::STBI {

bool ModuleInit() {
    AddTextureImporter("STBI", std::unique_ptr<TextureImporter>(new STBITextureImporter()));
    return true;
}

void ModuleTerm() {
    RemoveTextureImporter("STBI");
}

TOON_MODULE {
    .Name       = "STBI",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::STBI