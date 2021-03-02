#include <Toon/Toon.hpp>

#include <Toon/Log.hpp>
#include <Toon/Module.hpp>
#include <Toon/GraphicsDriver.hpp>
#include <Toon/Time.hpp>
#include <Toon/Scene.hpp>
#include <Toon/Path.hpp>

// #include <Python/PyDusk.hpp>

// #include <Python.h>
// #include <frameobject.h>

#include <Toon/String.hpp>
#include <vector>
#include <cstdio>
#include <thread>

// #include <cflags.h>

namespace Toon {

TOON_ENGINE_API
bool Initialize(int argc, char ** argv) 
{
    /*cflags_t * flags = cflags_init();

    bool help = false;
    cflags_add_bool(flags, '\0', "help", &help, "Display this help and exit");

    bool verbose = false;
    cflags_add_bool(flags, 'v', "verbose", &verbose, "Enable verbose logging");

    cflags_parse(flags, argc, argv);

    if (help) {
        cflags_print_usage(flags,
            "[OPTION]...",
            "Toon game and simulation engine",
            "Additional information about this library can be found at:\n"
            "  https://github.com/WhoBrokeTheBuild/Toon"
        );
    }

    const char * envVerbose = getenv("TOON_VERBOSE");

    if (verbose || envVerbose) {
        ToonLogInfo("Enabling verbose logging");
        SetVerboseLoggingEnabled(true);
    }

    InitMemoryTracking();

    PyImport_AppendInittab("Toon", PyInit_Toon);

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program) {
        Py_SetProgramName(program);
    }

    PyMem_RawFree(program);
    
    Py_Initialize();
    PyImport_ImportModule("Toon");*/
    
    ToonLogVerbose("Toon Version: %s", GetVersion().GetString());
    ToonLogVerbose("Application Name: %s", GetApplicationName());
    ToonLogVerbose("Application Version: %s", GetApplicationVersion().GetString());

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