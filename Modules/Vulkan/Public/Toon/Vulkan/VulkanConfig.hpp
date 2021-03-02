#ifndef TOON_VULKAN_CONFIG_HPP
#define TOON_VULKAN_CONFIG_HPP

#include <Toon/Config.hpp>

#if defined(TOON_VULKAN_EXPORT)
    #define TOON_VULKAN_API TOON_API_EXPORT
#else
    #define TOON_VULKAN_API TOON_API_IMPORT
#endif

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glad/vulkan.h>

#include <Toon/SDL2/SDL2Config.hpp>
#include <SDL_vulkan.h>

DISABLE_WARNINGS()

    #include <vk_mem_alloc.h>

ENABLE_WARNINGS()

#endif // TOON_VULKAN_CONFIG_HPP