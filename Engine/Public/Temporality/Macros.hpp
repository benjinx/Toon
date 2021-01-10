#ifndef TEMPORALITY_MACROS_HPP
#define TEMPORALITY_MACROS_HPP

#include <Temporality/Platform.hpp>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
    TypeName(const TypeName&) = delete;             \
    TypeName& operator=(const TypeName&) = delete;

#if defined(TEMPORALITY_COMPILER_MSVC)
    #define TEMPORALITY_FUNCTION_NAME() __FUNCSIG__
#elif defined(TEMPORALITY_COMPILER_GCC) || defined(TEMPORALITY_COMPILER_CLANG)
    #define TEMPORALITY_FUNCTION_NAME() __PRETTY_FUNCTION__
#else
    #define TEMPORALITY_FUNCTION_NAME() __func__
#endif

#endif // TEMPORALITY_MACROS_HPP