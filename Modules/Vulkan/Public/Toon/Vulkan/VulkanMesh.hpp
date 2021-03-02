#ifndef TOON_VULKAN_MESH_HPP
#define TOON_VULKAN_MESH_HPP

#include <Toon/Vulkan/VulkanConfig.hpp>

#include <Toon/Mesh.hpp>

namespace Toon::Vulkan {

#define TOON_VULKAN_MESH(x) (dynamic_cast<Toon::Vulkan::VulkanMesh *>(x))

class TOON_VULKAN_API VulkanMesh : public Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanMesh)

    VulkanMesh() = default;

    virtual ~VulkanMesh() = default;

    bool Initialize() override;

    void Terminate() override;

    bool Create();

    void Destroy();

    void Render(RenderContext * ctx) override;

private:

    VkDescriptorSet _vkDescriptorSet;

}; // class VulkanMesh

} // namespace Toon::Vulkan

#endif // TOON_VULKAN_MESH_HPP