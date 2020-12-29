#ifndef TEMPORALITY_MODULE_HPP
#define TEMPORALITY_MODULE_HPP

#include <Temporality/Config.hpp>

#include <string>

namespace Temporality {

struct TEMPORALITY_ENGINE_API TemporalityModule {
    const char * Name;
    bool (*Initialize)();
    void (*Terminate)();
};

TEMPORALITY_ENGINE_API
bool LoadModule(const std::string& name);

TEMPORALITY_ENGINE_API
void FreeModule(const std::string& name);

TEMPORALITY_ENGINE_API
void FreeAllModules();

#define TEMPORALITY_MODULE \
    extern "C" TEMPORALITY_API_EXPORT struct TemporalityModule _TemporalityModule =

}

#endif // TEMPORALITY_MODULE_HPP