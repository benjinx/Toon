#include <Temporality/Module.hpp>

#if defined(TEMPORALITY_OS_WINDOWS)
    #include <Windows.h>
    typedef HMODULE ModuleHandle;
#else
    #include <dlfcn.h>
    typedef void * ModuleHandle;
#endif

#include <vector>

namespace Temporality {

std::vector<ModuleHandle> _gModules;

bool LoadModule(const std::string& name)
{
    ModuleHandle module = nullptr;
    
    #if defined(TEMPORALITY_OS_WINDOWS)
        module = LoadLibraryA(name.c_str());
        if (!module) {
            fprintf(stderr, "Failed to load module. %s\n", name.c_str());
            return false;
        }

        TemporalityModule * def = (TemporalityModule *)GetProcAddress(module, "_TemporalityModule");

    #else
        std::string filename = "lib" + name + ".so";
        module = dlopen(filename.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!module) {
            fprintf(stderr, "Failed to load module. %s : %s\n", filename.c_str(), dlerror());
            return false;
        }

        TemporalityModule * def = (TemporalityModule *)dlsym(module, "_TemporalityModule");
    #endif

    if (!def) {
        fprintf(stderr, "Failed to find _TemporalityModule.\n");
        return false;
    }

    _gModules.push_back(module);

    if (def->Init) {
        def->Init();
    }

    return true;
}

void FreeModules()
{

    for (auto module : _gModules) {

        #if defined(TEMPORALITY_OS_WINDOWS)
            TemporalityModule * def = (TemporalityModule *)GetProcAddress(module, "_TemporalityModule");
        #else
            TemporalityModule * def = (TemporalityModule *)dlsym(module, "_TemporalityModule");
        #endif

        if (def && def->Term) {
            def->Term();
        }

        #if defined(TEMPORALITY_OS_WINDOWS)
            FreeLibrary(module);
        #else
            dlclose(module);
        #endif
    }

    _gModules.clear();
}

}