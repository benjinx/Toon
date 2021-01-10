#ifndef TEMPORALITY_VULKAN_CONFIG_HPP
#define TEMPORALITY_VULKAN_CONFIG_HPP

#include <Temporality/Config.hpp>

#if defined(TEMPORALITY_VULKAN_EXPORT)
    #define TEMPORALITY_VULKAN_API TEMPORALITY_API_EXPORT
#else
    #define TEMPORALITY_VULKAN_API TEMPORALITY_API_IMPORT
#endif

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glad/vulkan.h>

#include <Temporality/SDL2/SDL2Config.hpp>
#include <SDL_vulkan.h>

#endif // TEMPORALITY_VULKAN_CONFIG_HPP