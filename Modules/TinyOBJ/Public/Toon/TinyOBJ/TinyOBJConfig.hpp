#ifndef TOON_TINYOBJ_CONFIG_HPP
#define TOON_TINYOBJ_CONFIG_HPP

#include <Toon/Config.hpp>

#if defined(TOON_TINYOBJ_EXPORT)
    #define TOON_TINYOBJ_API TOON_API_EXPORT
#else
    #define TOON_TINYOBJ_API TOON_API_IMPORT
#endif

#endif // TOON_TINYOBJ_CONFIG_HPP