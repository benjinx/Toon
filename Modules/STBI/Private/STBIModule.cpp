#include <Toon/Module.hpp>

#include <Toon/STBI/STBITextureImporter.hpp>
#include <Toon/Log.hpp>

namespace Toon::STBI {

bool ModuleInit() {
    AddTextureImporter("STBI", std::unique_ptr<TextureImporter>(new STBITextureImporter()));
    ToonLogInfo("Init");

    return true;
}

void ModuleTerm() {
    RemoveTextureImporter("STBI");
    ToonLogInfo("Term()");
}

TOON_MODULE {
    .Name       = "STBI",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Toon::STBI