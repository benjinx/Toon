#ifndef TEMPORALITY_SDL2_CONFIG_HPP
#define TEMPORALITY_SDL2_CONFIG_HPP

#include <Temporality/Config.hpp>

#if defined(TEMPORALITY_SDL2_EXPORT)
    #define TEMPORALITY_SDL2_API TEMPORALITY_API_EXPORT
#else
    #define TEMPORALITY_SDL2_API TEMPORALITY_API_IMPORT
#endif

#endif // TEMPORALITY_SDL2_CONFIG_HPP