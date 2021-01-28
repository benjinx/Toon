#ifndef TOON_PLATFORM_HPP
#define TOON_PLATFORM_HPP

#if defined(_WIN32)

    // Windows
    #define TOON_PLATFORM_WINDOWS

#elif defined(__APPLE__) && defined(__MACH__)

    // Apple
    #include "TargetConditionals.h"

    #if TARGET_PLATFORM_IPHONE || TARGET_IPHONE_SIMULATOR
        // iPhone iOS
        #define TOON_PLATFORM_IOS
    #elif TARGET_PLATFORM_MAC
        // Mac OSX
        #define TOON_PLATFORM_OSX
    #endif

#elif defined(__unix__)

    #if defined(__ANDROID__)
        // Android
        #define TOON_PLATFORM_ANDROID
    #elif defined(__linux__)
        // Linux
        #define TOON_PLATFORM_LINUX
    #else
        #error Unsupported UNIX Operating System
    #endif

#else

    // Unknown
    #error Unsupported Operating System

#endif

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define TOON_COMPILER_MSVC _MSC_VER

#elif defined(__GNUC__)
    
    // GNU Compiler Collection
    #define TOON_COMPILER_GCC __GNUC__

#elif defined(__clang__)

    // Apple Clang Compiler
    #define TOON_COMPILER_CLANG __clang__

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(NDEBUG)

    #define TOON_BUILD_RELEASE

#else

    #define TOON_BUILD_DEBUG

#endif

#if defined(TOON_PLATFORM_WINDOWS)

    #define TOON_PATH_SLASH '\\'
    #define TOON_PATH_SEPARATOR ';'

    // Disable min/max defines
    #define NOMINMAX

    // Disable python defines for snprintf/vsnprintf
    #define HAVE_SNPRINTF

    // Disable extra includes from Windows.h
    #define VC_EXTRALEAN

    #include <Windows.h>

    #undef FreeModule
    #undef CreateWindow
    
    #undef OPAQUE

#else

    #define TOON_PATH_SLASH '/'
    #define TOON_PATH_SEPARATOR ':'

#endif

#endif // TOON_PLATFORM_HPP