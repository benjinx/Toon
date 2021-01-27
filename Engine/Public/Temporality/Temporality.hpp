#ifndef TEMPORALITY_HPP
#define TEMPORALITY_HPP

#include <Temporality/Config.hpp>
//#include <Temporality/App.hpp>
#include <Temporality/Camera.hpp>
//#include <Temporality/DevUI.hpp>
//#include <Temporality/Event.hpp>
#include <Temporality/Scene/Entity.hpp>
#include <Temporality/RenderContext.hpp>
#include <Temporality/UpdateContext.hpp>
#include <Temporality/Scene/Component.hpp>
#include <Temporality/Scene/MeshComponent.hpp>
//#include <Temporality/Input.hpp>
//#include <Temporality/Light.hpp>
#include <Temporality/Log.hpp>
//#include <Temporality/Material.hpp>
#include <Temporality/Mesh.hpp>
//#include <Temporality/StaticMeshComponent.hpp>
//#include <Temporality/RiggedMeshComponent.hpp>
//#include <Temporality/Scene.hpp>
#include <Temporality/Shader.hpp>
#include <Temporality/Texture.hpp>
#include <Temporality/Utils.hpp>
#include <Temporality/Module.hpp>
#include <Temporality/GraphicsDriver.hpp>
#include <Temporality/TextureImporter.hpp>
#include <Temporality/MeshImporter.hpp>
#include <Temporality/Version.hpp>

#include <string>

//#include <imgui/imgui.h>

namespace Temporality {

TEMPORALITY_ENGINE_API
bool Initialize(int argc, char ** argv);

TEMPORALITY_ENGINE_API
void Terminate();

TEMPORALITY_ENGINE_API
bool IsRunning();

TEMPORALITY_ENGINE_API
void SetRunning(bool running);

TEMPORALITY_ENGINE_API
Version GetVersion();

TEMPORALITY_ENGINE_API
void SetApplicationName(const std::string& name);

TEMPORALITY_ENGINE_API
std::string GetApplicationName();

TEMPORALITY_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch);

TEMPORALITY_ENGINE_API
void SetApplicationVersion(const Version& version);

TEMPORALITY_ENGINE_API
Version GetApplicationVersion();

} // namespace Temporality

#endif // TEMPORALITY_HPP