#ifndef TEMPORALITY_PLATFORM_HPP
#define TEMPORALITY_PLATFORM_HPP

#if defined(_WIN32)

    // Windows
    #define TEMPORALITY_OS_WINDOWS

#elif defined(__APPLE__) && defined(__MACH__)

    // Apple
    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        // iPhone iOS
        #define TEMPORALITY_OS_IOS
    #elif TARGET_OS_MAC
        // Mac OSX
        #define TEMPORALITY_OS_OSX
    #endif

#elif defined(__unix__)

    #if defined(__ANDROID__)
        // Android
        #define TEMPORALITY_OS_ANDROID
    #elif defined(__linux__)
        // Linux
        #define TEMPORALITY_OS_LINUX
    #else
        #error Unsupported UNIX Operating System
    #endif

#else

    // Unknown
    #error Unsupported Operating System

#endif

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define TEMPORALITY_COMPILER_MSVC _MSC_VER

#elif defined(__GNUC__)
    
    // GNU Compiler Collection
    #define TEMPORALITY_COMPILER_GCC __GNUC__

#elif defined(__clang__)

    // Apple Clang Compiler
    #define TEMPORALITY_COMPILER_CLANG __clang__

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(NDEBUG)

    #define TEMPORALITY_BUILD_RELEASE

#else

    #define TEMPORALITY_BUILD_DEBUG

#endif

#if defined(TEMPORALITY_PLATFORM_WINDOWS)

    #define TEMPORALITY_PATH_SLASH '\\'
    #define TEMPORALITY_PATH_SEPARATOR ';'

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

    #define TEMPORALITY_PATH_SLASH '/'
    #define TEMPORALITY_PATH_SEPARATOR ':'

#endif

#endif // TEMPORALITY_PLATFORM_HPP