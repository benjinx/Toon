#ifndef TOON_CONFIG_HPP
#define TOON_CONFIG_HPP

#if _MSC_VER
    #define TOON_API_EXPORT __declspec(dllexport)
    #define TOON_API_IMPORT __declspec(dllimport)
#else
    #if __GNUC__ >= 4 || __clang__
        #define TOON_API_EXPORT __attribute__ ((__visibility__ ("default")))
        #define TOON_API_IMPORT __attribute__ ((__visibility__ ("default")))
    #endif
#endif

#if defined(TOON_ENGINE_EXPORT)
    #define TOON_ENGINE_API TOON_API_EXPORT
#else
    #define TOON_ENGINE_API TOON_API_IMPORT
#endif

#include <Toon/Macros.hpp>

#endif // TOON_CONFIG_HPP