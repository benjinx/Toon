#ifndef TEMPORALITY_CONFIG_HPP
#define TEMPORALITY_CONFIG_HPP

#if _MSC_VER
    #define TEMPORALITY_API_EXPORT __declspec(dllexport)
    #define TEMPORALITY_API_IMPORT __declspec(dllimport)
#else
    #if __GNUC__ >= 4 || __clang__
        #define TEMPORALITY_API_EXPORT __attribute__ ((__visibility__ ("default")))
        #define TEMPORALITY_API_IMPORT __attribute__ ((__visibility__ ("default")))
    #endif
#endif

#if defined(TEMPORALITY_ENGINE_EXPORT)
    #define TEMPORALITY_ENGINE_API TEMPORALITY_API_EXPORT
#else
    #define TEMPORALITY_ENGINE_API TEMPORALITY_API_IMPORT
#endif

#include <Temporality/Macros.hpp>

#endif // TEMPORALITY_CONFIG_HPP