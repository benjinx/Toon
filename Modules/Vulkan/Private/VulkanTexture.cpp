#include <Toon/Vulkan/VulkanTexture.hpp>

#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/Vulkan/VulkanGraphicsDriver.hpp>

namespace Toon::Vulkan {

TOON_VULKAN_API
VulkanTexture::~VulkanTexture()
{

}

TOON_VULKAN_API
bool VulkanTexture::Load(const std::unique_ptr<TextureData>& data, Options opts /*= Options()*/)
{
    ToonBenchmarkStart();

    // VulkanGraphicsDriver * gfx = TOON_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    // const auto& size = data->GetSize();
    // VkDeviceSize imageSize = size.x * size.y * data->GetComponents();

    // VkImageCreateInfo imageCreateInfo = {
    //     .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    //     .pNext = nullptr,
    //     .flags = 0,
    //     .imageType = VK_IMAGE_TYPE_2D,
    //     .format = GetVkDataFormat(data->GetComponents(), data->GetDataType()),
    //     .extent = {
    //         .width = size.x,
    //         .height = size.y,
    //         .depth = 1,
    //     },
    //     .mipLevels = 1,
    //     .arrayLayers = 1,
    //     .samples = VK_SAMPLE_COUNT_1_BIT,
    //     .tiling = VK_IMAGE_TILING_OPTIMAL,
    //     .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    //     .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    //     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    // };

    // if (vkCreateImage(gfx->GetDevice(), &imageCreateInfo, nullptr, &_vkImage) != VK_SUCCESS) {
    //     ToonLogError("Failed to create image");
    //     return false;
    // }

    // VkDeviceMemory memory;

    // TODO:

    ToonBenchmarkEnd();
    return true;
}

} // namespace Toon::Vulkan
