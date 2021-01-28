#ifndef TOON_MACROS_HPP
#define TOON_MACROS_HPP

#include <Toon/Platform.hpp>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
    TypeName(const TypeName&) = delete;             \
    TypeName& operator=(const TypeName&) = delete;

#if defined(TOON_COMPILER_MSVC)
    #define TOON_FUNCTION_NAME() __FUNCSIG__
#elif defined(TOON_COMPILER_GCC) || defined(TOON_COMPILER_CLANG)
    #define TOON_FUNCTION_NAME() __PRETTY_FUNCTION__
#else
    #define TOON_FUNCTION_NAME() __func__
#endif

#endif // TOON_MACROS_HPP