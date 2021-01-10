#ifndef TEMPORALITY_OPENGL_CONFIG_HPP
#define TEMPORALITY_OPENGL_CONFIG_HPP

#include <Temporality/Config.hpp>

#if defined(TEMPORALITY_OPENGL_EXPORT)
    #define TEMPORALITY_OPENGL_API TEMPORALITY_API_EXPORT
#else
    #define TEMPORALITY_OPENGL_API TEMPORALITY_API_IMPORT
#endif

#include <glad/gl.h>

#include <Temporality/SDL2/SDL2Config.hpp>
#include <SDL_opengl.h>

#endif // TEMPORALITY_OPENGL_CONFIG_HPP