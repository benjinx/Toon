#ifndef TEMPORALITY_VULKAN_PRIMITIVE_HPP
#define TEMPORALITY_VULKAN_PRIMITIVE_HPP

#include <Temporality/Vulkan/VulkanConfig.hpp>

#include <Temporality/Log.hpp>
#include <Temporality/Primitive.hpp>
#include <Temporality/PrimitiveData.hpp>
#include <Temporality/Buffer.hpp>

#include <vector>
#include <optional>

namespace Temporality::Vulkan {

#define TEMPORALITY_VULKAN_PRIMITIVE(x) (dynamic_cast<Temporality::Vulkan::VulkanPrimitive *>(x))

class TEMPORALITY_VULKAN_API VulkanPrimitive : public Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPrimitive)

    VulkanPrimitive() = default;

    virtual ~VulkanPrimitive();

    bool Load(const std::unique_ptr<PrimitiveData>& data) override;

    VkPrimitiveTopology GetVkPrimitiveTopology() {
        return _vkPrimitiveTopology;
    }
    
    void GenerateCommands(VkCommandBuffer vkCommandBuffer);

private:

    std::unique_ptr<Buffer> _indexBuffer;

    std::unique_ptr<Buffer> _vertexBuffer;

    VkPrimitiveTopology _vkPrimitiveTopology;

    bool _indexed = false;

    uint32_t _count = 0;

}; // class VulkanPrimitive

inline std::optional<VkPrimitiveTopology> GetVkPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::PointList:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveTopology::LineList:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveTopology::LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveTopology::TriangleList:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveTopology::TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }

    return {};
}

} // namespace Temporality::Vulkan

#endif // TEMPORALITY_VULKAN_PRIMITIVE_HPP