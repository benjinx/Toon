#ifndef TOON_MACROS_HPP
#define TOON_MACROS_HPP

#include <Toon/Platform.hpp>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
    TypeName(const TypeName&) = delete;             \
    TypeName& operator=(const TypeName&) = delete;

#define TOON_STRINGIFY(x) _TOON_STRINGIFY(x)
#define _TOON_STRINGIFY(x) #x

#if defined(TOON_COMPILER_MSVC)
    #define TOON_FUNCTION_NAME() __FUNCSIG__
#elif defined(TOON_COMPILER_GCC) || defined(TOON_COMPILER_CLANG)
    #define TOON_FUNCTION_NAME() __PRETTY_FUNCTION__
#else
    #define TOON_FUNCTION_NAME() __func__
#endif

#if defined(TOON_COMPILER_MSVC)
    #define DISABLE_WARNINGS() \
        _Pragma("warning(push, 0)")
    
    #define ENABLE_WARNINGS() \
        _Pragma("warning(pop)")
#elif defined(TOON_COMPILER_GCC)
    #define DISABLE_WARNINGS() \
        _Pragma("GCC diagnostic push")                      \
        _Pragma("GCC diagnostic ignored \"-Wall\"")
    
    #define ENABLE_WARNINGS() \
        _Pragma("GCC diagnostic pop")
#elif defined(TOON_COMPILER_CLANG)
    #define DISABLE_WARNINGS() \
        _Pragma("clang diagnostic push")                    \
        _Pragma("clang diagnostic ignored \"-Wall\"")
    
    #define ENABLE_WARNINGS() \
        _Pragma("clang diagnostic pop")
#else
    #define DISABLE_WARNINGS()
#endif

#endif // TOON_MACROS_HPP