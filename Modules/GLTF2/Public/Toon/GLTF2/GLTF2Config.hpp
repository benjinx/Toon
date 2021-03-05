#ifndef TOON_GLTF2_CONFIG_HPP
#define TOON_GLTF2_CONFIG_HPP

#include <Toon/Config.hpp>

#if defined(TOON_GLTF2_EXPORT)
    #define TOON_GLTF2_API TOON_API_EXPORT
#else
    #define TOON_GLTF2_API TOON_API_IMPORT
#endif

#endif // TOON_GLTF2_CONFIG_HPP