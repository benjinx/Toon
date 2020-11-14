#ifndef MACROS_HPP
#define MACROS_HPP

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;   \
    TypeName& operator=(const TypeName&) = delete;

#define UNUSED(X) (void)(X)

#endif // MACROS_HPP