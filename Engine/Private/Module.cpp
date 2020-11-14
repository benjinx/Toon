#include <Temporality/Module.hpp>

#if defined(_WIN32)
    #include <Windows.h>
    typedef HMODULE ModuleHandle;
#else
    #include <dlfcn.h>
    typedef void * ModuleHandle;
#endif

#include <vector>

std::vector<ModuleHandle> _gModules;

bool LoadModule(const std::string& name)
{
    ModuleHandle module = nullptr;
    
    std::string filename = "lib" + name + ".so";
    
    module = dlopen(filename.c_str(), RTLD_GLOBAL | RTLD_NOW);
    if (!module) {
        fprintf(stderr, "Failed to load module. %s : %s\n", filename.c_str(), dlerror());
        return false;
    }

    TemporalityModule * def = (TemporalityModule *)dlsym(module, "_TemporalityModule");
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
        TemporalityModule * def = (TemporalityModule *)dlsym(module, "_TemporalityModule");

        if (def && def->Term) {
            def->Term();
        }

        dlclose(module);
    }

    _gModules.clear();
}