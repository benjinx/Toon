#ifndef TOON_HPP
#define TOON_HPP

#include <Toon/Config.hpp>
#include <Toon/Version.hpp>
#include <Toon/String.hpp>

#include <functional>
#include <Toon/Path.hpp>
// #include <Toon/Camera.hpp>
// #include <Toon/Entity.hpp>
// #include <Toon/RenderContext.hpp>
// #include <Toon/UpdateContext.hpp>
// #include <Toon/Component.hpp>
// #include <Toon/MeshComponent.hpp>
// #include <Toon/Light.hpp>
// #include <Toon/Log.hpp>
// #include <Toon/Mesh.hpp>
// #include <Toon/Shader.hpp>
// #include <Toon/Texture.hpp>
// #include <Toon/Util.hpp>
// #include <Toon/GraphicsDriver.hpp>
// #include <Toon/TextureImporter.hpp>
// #include <Toon/MeshImporter.hpp>
// #include <Toon/Version.hpp>

namespace Toon {

TOON_ENGINE_API
bool Initialize(int argc, char ** argv);

TOON_ENGINE_API
void Terminate();

TOON_ENGINE_API
void LoadConfigurationFile(const Path& path, string_view configName);

TOON_ENGINE_API
void SetRunning(bool running);

TOON_ENGINE_API
bool IsRunning();

TOON_ENGINE_API
bool Run(std::function<void()> update);

TOON_ENGINE_API
bool RunScriptFile(const string& filename);

TOON_ENGINE_API
bool RunScriptString(const string& code);

TOON_ENGINE_API
Version GetVersion();

TOON_ENGINE_API
void SetApplicationName(const string& name);

TOON_ENGINE_API
string GetApplicationName();

TOON_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch);

TOON_ENGINE_API
void SetApplicationVersion(const Version& version);

TOON_ENGINE_API
Version GetApplicationVersion();

} // namespace Toon

#endif // TOON_HPP