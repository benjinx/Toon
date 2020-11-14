#ifndef TEMPORALITY_MODULE_HPP
#define TEMPORALITY_MODULE_HPP

#include <string>

struct TemporalityModule {
    const char * Name;
    void (*Init)();
    void (*Term)();
};

bool LoadModule(const std::string& name);

void FreeModules();

#endif // TEMPORALITY_MODULE_HPP