#ifndef TOON_MODULE_HPP
#define TOON_MODULE_HPP

#include <Toon/Config.hpp>
#include <Toon/Version.hpp>

#include <Toon/String.hpp>

#include <initializer_list>

namespace Toon {

struct TOON_ENGINE_API ToonModule {
    const char * Name;
    bool (*Initialize)();
    void (*Terminate)();
};

TOON_ENGINE_API
bool LoadModule(const string& name);

TOON_ENGINE_API
void FreeModule(const string& name);

TOON_ENGINE_API
void FreeAllModules();

#define TOON_MODULE \
    extern "C" TOON_API_EXPORT struct ToonModule _ToonModule =

}

#endif // TOON_MODULE_HPP