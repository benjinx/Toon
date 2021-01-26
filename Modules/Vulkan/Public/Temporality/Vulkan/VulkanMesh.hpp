#ifndef TEMPORALITY_VULKAN_MESH_HPP
#define TEMPORALITY_VULKAN_MESH_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>

#include <Temporality/Mesh.hpp>

namespace Temporality::Vulkan {

#define TEMPORALITY_VULKAN_MESH(x) (dynamic_cast<Temporality::Vulkan::VulkanMesh *>(x))

class TEMPORALITY_VULKAN_API VulkanMesh : public Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanMesh)

    VulkanMesh() = default;

    virtual ~VulkanMesh() = default;

    bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data) override;
    
    void Render(RenderContext * ctx) override;

    void GenerateCommands(VkCommandBuffer vkCommandBuffer);

private:


}; // class VulkanMesh

} // namespace Temporality::Vulkan

#endif // TEMPORALITY_VULKAN_MESH_HPP