#include <Temporality/Module.hpp>

#include <Temporality/STBI/STBITextureImporter.hpp>
#include <Temporality/Log.hpp>

namespace Temporality::STBI {

void ModuleInit() {
    AddTextureImporter("STBI", std::unique_ptr<TextureImporter>(new STBITextureImporter()));
    LogInfo("Init");
}

void ModuleTerm() {
    RemoveTextureImporter("STBI");
    LogInfo("Term()");
}

TEMPORALITY_MODULE {
    .Name       = "STBI",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
};

} // namespace Temporality::STBI