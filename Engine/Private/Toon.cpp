#include <Toon/Toon.hpp>

#include <Toon/Log.hpp>
#include <Toon/Module.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Time.hpp>
#include <Toon/Scene.hpp>

// #include <Python/PyDusk.hpp>

// #include <Python.h>
// #include <frameobject.h>

#include <Toon/String.hpp>

#include <Toon/JSON.hpp>
#include <vector>
#include <cstdio>
#include <thread>

#include <cflags.h>

#include <fstream>

namespace Toon {

TOON_ENGINE_API
bool Initialize(int argc, char ** argv) 
{
    cflags_t * flags = cflags_init();

    bool help = false;
    cflags_add_bool(flags, '\0', "help", &help, "Display this help and exit");

    const char * config = NULL;
    cflags_add_string(flags, 'c', "config", &config, "Configuration to use");

    cflags_parse(flags, argc, argv);

    if (help) {
        cflags_print_usage(flags,
            "[OPTION]...",
            "Toon game and simulation engine",
            "Additional information about this library can be found at:\n"
            "  https://github.com/WhoBrokeTheBuild/Toon"
        );
    }

    if (flags->argc > 1) {
        LoadConfigurationFile(flags->argv[1], (config ? config : ""));
    }

    // InitMemoryTracking();

    // PyImport_AppendInittab("Toon", PyInit_Toon);

    // wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    // if (program) {
    //     Py_SetProgramName(program);
    // }

    // PyMem_RawFree(program);
    
    // Py_Initialize();

    // PyImport_ImportModule("Toon");
    
    ToonLogVerbose("Toon Version: %s", GetVersion());
    ToonLogVerbose("Application Name: %s", GetApplicationName());
    ToonLogVerbose("Application Version: %s", GetApplicationVersion());

    return true;
}

TOON_ENGINE_API
void Terminate()
{
    //Py_Finalize();

    FreeAllModules();

    //TermMemoryTracking();
    
    //CloseAllLogFiles();
}

TOON_ENGINE_API
void LoadConfigurationFile(const Path& path, string_view configName)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        ToonLogFatal("No.");
    }

    json data;
    data << file;

    auto applyConfig = [](json& data) {
        const auto& modules = data.value<std::vector<string>>("modules", {});
        for (const auto& name : modules) {
            LoadModule(name);
        }
    };

    SetApplicationName(data.value("name", ""));
    SetApplicationVersion(data.value("version", ""));

    applyConfig(data);

    if (!configName.empty()) {
        auto it = data.find("configurations");
        if (it != data.end()) {
            auto& configurations = it.value();
            if (configurations.is_object()) {
                it = configurations.find(configName);
                if (it != configurations.end()) {
                    applyConfig(it.value());
                }
            }
        }
    }
}

static bool _Running = true;

TOON_ENGINE_API
bool IsRunning()
{
    return _Running;
}

TOON_ENGINE_API
void SetRunning(bool running)
{
    _Running = running;
}

TOON_ENGINE_API
bool Run(std::function<void()> update)
{
    using namespace std::chrono;

    GraphicsDriver * gfx = GetGraphicsDriver();
    UpdateContext * updateCtx = gfx->GetUpdateContext();

    high_resolution_clock::time_point startTime = high_resolution_clock::now();
    high_resolution_clock::time_point previousTime = startTime;

    Toon::SetRunning(true);

    while (Toon::IsRunning()) {
        high_resolution_clock::time_point currentTime = high_resolution_clock::now();
        auto totalDuration = duration_cast<milliseconds>(currentTime - startTime);
        auto previousFrameDuration = duration_cast<microseconds>(currentTime - previousTime);
        previousTime = currentTime;

        updateCtx->SetTotalDuration(totalDuration);
        updateCtx->SetPreviousFrameDuration(previousFrameDuration);
        auto frameEndTime = currentTime + updateCtx->GetExpectedFrameDuration();

        gfx->ProcessEvents();

        update();

        auto scene = GetCurrentScene();
        if (scene) {
            scene->Update(gfx->GetUpdateContext());
        }

        gfx->Render();

        currentTime = high_resolution_clock::now();
        auto timeToSleep = duration_cast<milliseconds>(frameEndTime - currentTime);
        if (timeToSleep > 1ms) { // TODO: Find "minimum" sleep time
            std::this_thread::sleep_for(timeToSleep);
        }
    }

    return true;
}

TOON_ENGINE_API
bool RunScriptFile(const string& filename)
{
    FILE * file = nullptr;

    const auto& assetPaths = GetAssetPathList();

    for (const auto& path : assetPaths) {
        Path fullPath = path / "Scripts" / filename;
        ToonLogVerbose("Checking '%s'", fullPath);

        file = fopen(fullPath.ToCString(), "rt");
        if (file) {
            break;
        }
    }

    if (!file) {
        ToonLogError("Failed to run script file '%s'", filename);
    }

    // PyObject * pyMain = PyImport_AddModule("__main__");
    // PyObject * pyMainDict = PyModule_GetDict(pyMain);
    // PyObject * pyLocalDict = PyDict_New();

    // PyRun_File(file, filename.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    // PyCheckError();

    fclose(file);
    return true;
}

TOON_ENGINE_API
bool RunScriptString(const string& code)
{
    // PyObject * pyMain = PyImport_AddModule("__main__");
    // PyObject * pyMainDict = PyModule_GetDict(pyMain);
    // PyObject * pyLocalDict = PyDict_New();

    // PyRun_String(code.c_str(), Py_single_input, pyMainDict, pyLocalDict);

    // PyCheckError();
    return true;
}

TOON_ENGINE_API
Version GetVersion()
{
    return Version(TOON_VERSION_MAJOR, TOON_VERSION_MINOR, TOON_VERSION_PATCH);
}

static string _ApplicationName;

TOON_ENGINE_API
void SetApplicationName(const string& name)
{
    _ApplicationName = name;
}

TOON_ENGINE_API
string GetApplicationName()
{
    return _ApplicationName;
}

static Version _ApplicationVersion;

TOON_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch)
{
    _ApplicationVersion = Version(major, minor, patch);
}

TOON_ENGINE_API
void SetApplicationVersion(const Version& version)
{
    _ApplicationVersion = version;
}

TOON_ENGINE_API
Version GetApplicationVersion()
{
    return _ApplicationVersion;
}

} // namespace Toon