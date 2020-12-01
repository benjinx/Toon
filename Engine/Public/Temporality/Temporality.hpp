#ifndef TEMPORALITY_HPP
#define TEMPORALITY_HPP

#include <Temporality/Config.hpp>
//#include <Temporality/App.hpp>
//#include <Temporality/Camera.hpp>
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
#include <Temporality/Graphics/Mesh.hpp>
//#include <Temporality/StaticMeshComponent.hpp>
//#include <Temporality/RiggedMeshComponent.hpp>
//#include <Temporality/Scene.hpp>
#include <Temporality/Graphics/Shader.hpp>
#include <Temporality/Graphics/Texture.hpp>
#include <Temporality/Utils.hpp>
#include <Temporality/Module.hpp>
#include <Temporality/Graphics/GraphicsDriver.hpp>
#include <Temporality/Graphics/TextureImporter.hpp>
#include <Temporality/Graphics/MeshImporter.hpp>

//#include <imgui/imgui.h>

namespace Temporality {

TEMPORALITY_ENGINE_API
bool IsRunning();

TEMPORALITY_ENGINE_API
void SetRunning(bool running);

} // namespace Temporality

#endif // TEMPORALITY_HPP