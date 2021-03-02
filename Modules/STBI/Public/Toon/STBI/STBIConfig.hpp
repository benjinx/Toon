#ifndef TOON_STBI_CONFIG_HPP
#define TOON_STBI_CONFIG_HPP

#include <Toon/Config.hpp>

#if defined(TOON_STBI_EXPORT)
    #define TOON_STBI_API TOON_API_EXPORT
#else
    #define TOON_STBI_API TOON_API_IMPORT
#endif

#define STB_NO_HDR
#define STB_NO_PSD
#define STB_NO_PIC
#define STB_NO_PNM
#include <stb/stb_image.h>

#endif // TOON_STBI_CONFIG_HPP