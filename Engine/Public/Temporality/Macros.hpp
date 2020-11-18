#ifndef TEMPORALITY_MACROS_HPP
#define TEMPORALITY_MACROS_HPP

#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
    TypeName(const TypeName&) = delete;             \
    TypeName& operator=(const TypeName&) = delete;

#endif // TEMPORALITY_MACROS_HPP