#include <Toon/Toon.hpp>

namespace Toon {

static bool _Running = true;

TOON_ENGINE_API
bool Initialize(int argc, char ** argv) 
{
    LogVerbose("Toon Version: %s", GetVersion().GetString());
    LogVerbose("Application Name: %s", GetApplicationName());
    LogVerbose("Application Version: %s", GetApplicationVersion().GetString());

    return true;
}

TOON_ENGINE_API
void Terminate()
{
    FreeAllModules();
}

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
Version GetVersion()
{
    return Version(1, 0, 0);
}

static std::string _ApplicationName;

TOON_ENGINE_API
void SetApplicationName(const std::string& name)
{
    _ApplicationName = name;
}

TOON_ENGINE_API
std::string GetApplicationName()
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