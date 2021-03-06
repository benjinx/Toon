#include <Toon/Module.hpp>
#include <Toon/Log.hpp>

#if defined(TOON_PLATFORM_WINDOWS)
    typedef HMODULE ModuleHandle;
#else
    #include <dlfcn.h>
    typedef void * ModuleHandle;
#endif

#include <unordered_map>

namespace Toon {

std::unordered_map<string, ModuleHandle> _gModules;

ModuleHandle _dlopen(const string& filename)
{
    ModuleHandle handle = nullptr;

    #if defined(TOON_PLATFORM_WINDOWS)

        ToonLogVerbose("Loading Module from PATH: '%s'", getenv("PATH"));

        handle = LoadLibraryA(filename.c_str());
        if (!handle) {
            //WindowsErrorMessage msg(HRESULT_FROM_WIN32(GetLastError()));
            //ToonLogError("Failed to load '%s', %s", filename, msg);
            return nullptr;
        }
        
    #else

        ToonLogVerbose("Loading Module from LD_LIBRARY_PATH: '%s'", getenv("LD_LIBRARY_PATH"));

        #if defined(TOON_PLATFORM_APPLE)

            string libFilename = "lib" + filename + ".dylib";

        #else

            string libFilename = "lib" + filename + ".so";

        #endif

        handle = dlopen(libFilename.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!handle) {
            ToonLogError("Failed to load '%s', %s", libFilename, dlerror());
            return nullptr;
        }

    #endif

    return handle;
}

void * _dlsym(ModuleHandle handle, const string& symbol)
{
    #if defined(TOON_PLATFORM_WINDOWS)

        return GetProcAddress(handle, symbol.c_str());

    #else

        return dlsym(handle, symbol.c_str());

    #endif
}

void _dlclose(ModuleHandle handle)
{
    #if defined(TOON_PLATFORM_WINDOWS)

        FreeLibrary(handle);

    #else

        dlclose(handle);

    #endif
}

bool LoadModule(const string& name)
{
    ToonLogLoad("Loading module '%s'", name);

    ModuleHandle handle = _dlopen(name);
    if (!handle) {
        return false;
    }

    ToonModule * def = static_cast<ToonModule *>(_dlsym(handle, "_ToonModule"));

    if (!def) {
        ToonLogError("Failed to find _Module symbol");
        return false;
    }

    if (def->Initialize) {
        if (!def->Initialize()) {
            _dlclose(handle);

            ToonLogError("Failed to initialize module '%s'", name);
            return false;
        }
    }

    _gModules.emplace(name, handle);

    return true;
}

void FreeModule(const string& name)
{
    auto it = _gModules.find(name);
    if (it == _gModules.end()) {
        ToonLogWarn("Failed to free module '%s', module is not loaded", name);
        return;
    }

    ModuleHandle handle = it->second;
    ToonModule * def = static_cast<ToonModule *>(_dlsym(handle, "_ToonModule"));

    if (def && def->Terminate) {
        def->Terminate();
    }

    _dlclose(handle);

    _gModules.erase(it);
}

void FreeAllModules()
{
    for (const auto& it : _gModules) {
        ModuleHandle handle = it.second;
        ToonModule * def = static_cast<ToonModule *>(_dlsym(handle, "_ToonModule"));
        if (def && def->Terminate) {
            def->Terminate();
        }

        _dlclose(handle);
    }

    _gModules.clear();
}

}