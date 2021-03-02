#ifndef TOON_VULKAN_TEXTURE_HPP
#define TOON_VULKAN_TEXTURE_HPP

#include <Toon/Vulkan/VulkanConfig.hpp>

#include <Toon/Texture.hpp>

namespace Toon::Vulkan {

#define TOON_VULKAN_TEXTURE(x) (dynamic_cast<Toon::Vulkan::VulkanTexture *>(x))

class TOON_VULKAN_API VulkanTexture : public Texture
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanTexture)

    VulkanTexture() = default;

    virtual ~VulkanTexture();

    bool Load(const std::unique_ptr<TextureData>& data, Options opts = Options()) override;

    VkImage GetVKImage() const;

private:

    VkImage _vkImage;
}; // class VulkanTexture

inline VkFormat GetVkDataFormat(int components, const TextureDataType& dataType)
{
    if (components == 1) {
        switch(dataType) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32_SINT;
        }
    }
    else if (components == 2) {
        switch(dataType) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8G8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8G8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16G16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16G16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32G32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32G32_SINT;
        }
    }
    else if (components == 3) {
        switch(dataType) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8G8B8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8G8B8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16G16B16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16G16B16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32G32B32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32G32B32_SINT;
        }
    }
    else if (components == 4) {
        switch(dataType) {
            case TextureDataType::UnsignedByte:
                return VK_FORMAT_R8G8B8A8_UINT;
            case TextureDataType::Byte:
                return VK_FORMAT_R8G8B8A8_SINT;
            case TextureDataType::UnsignedShort:
                return VK_FORMAT_R16G16B16A16_UINT;
            case TextureDataType::Short:
                return VK_FORMAT_R16G16B16A16_SINT;
            case TextureDataType::UnsignedInt:
                return VK_FORMAT_R32G32B32A32_UINT;
            case TextureDataType::Int:
                return VK_FORMAT_R32G32B32A32_SINT;
        }
    }

    return VK_FORMAT_UNDEFINED;
}

} // namespace Toon::Vulkan

#endif // TOON_VULKAN_TEXTURE_HPP
