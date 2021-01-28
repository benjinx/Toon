#ifndef TOON_OPENGL_CONFIG_HPP
#define TOON_OPENGL_CONFIG_HPP

#include <Toon/Config.hpp>

#if defined(TOON_OPENGL_EXPORT)
    #define TOON_OPENGL_API TOON_API_EXPORT
#else
    #define TOON_OPENGL_API TOON_API_IMPORT
#endif

#include <glad/gl.h>

#include <Toon/SDL2/SDL2Config.hpp>
#include <SDL_opengl.h>

#endif // TOON_OPENGL_CONFIG_HPP