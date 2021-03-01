#ifndef TOON_HPP
#define TOON_HPP

#include <Toon/Config.hpp>
//#include <Toon/App.hpp>
#include <Toon/Camera.hpp>
//#include <Toon/DevUI.hpp>
//#include <Toon/Event.hpp>
#include <Toon/Entity.hpp>
#include <Toon/RenderContext.hpp>
#include <Toon/UpdateContext.hpp>
#include <Toon/Component.hpp>
#include <Toon/MeshComponent.hpp>
//#include <Toon/Input.hpp>
#include <Toon/Light.hpp>
#include <Toon/Log.hpp>
//#include <Toon/Material.hpp>
#include <Toon/Mesh.hpp>
//#include <Toon/StaticMeshComponent.hpp>
//#include <Toon/RiggedMeshComponent.hpp>
//#include <Toon/Scene.hpp>
#include <Toon/Shader.hpp>
#include <Toon/Texture.hpp>
#include <Toon/Utils.hpp>
#include <Toon/Module.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/TextureImporter.hpp>
#include <Toon/MeshImporter.hpp>
#include <Toon/Version.hpp>

#include <string>

//#include <imgui/imgui.h>

namespace Toon {

TOON_ENGINE_API
bool Initialize(int argc, char ** argv);

TOON_ENGINE_API
void Terminate();

TOON_ENGINE_API
bool IsRunning();

TOON_ENGINE_API
void SetRunning(bool running);

TOON_ENGINE_API
Version GetVersion();

TOON_ENGINE_API
void SetApplicationName(const std::string& name);

TOON_ENGINE_API
std::string GetApplicationName();

TOON_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch);

TOON_ENGINE_API
void SetApplicationVersion(const Version& version);

TOON_ENGINE_API
Version GetApplicationVersion();

} // namespace Toon

#endif // TOON_HPP