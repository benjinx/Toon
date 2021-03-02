#ifndef TOON_SDL2_CONFIG_HPP
#define TOON_SDL2_CONFIG_HPP

#include <Toon/Config.hpp>

#if defined(TOON_SDL2_EXPORT)
    #define TOON_SDL2_API TOON_API_EXPORT
#else
    #define TOON_SDL2_API TOON_API_IMPORT
#endif

#include <SDL.h>

#endif // TOON_SDL2_CONFIG_HPP