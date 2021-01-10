#include <Temporality/Temporality.hpp>

namespace Temporality {

static bool _Running = true;

TEMPORALITY_ENGINE_API
bool IsRunning()
{
    return _Running;
}

TEMPORALITY_ENGINE_API
void SetRunning(bool running)
{
    _Running = running;
}

TEMPORALITY_ENGINE_API
Version GetVersion()
{
    return Version(1, 0, 0);
}

static std::string _ApplicationName;

TEMPORALITY_ENGINE_API
void SetApplicationName(const std::string& name)
{
    _ApplicationName = name;
}

TEMPORALITY_ENGINE_API
std::string GetApplicationName()
{
    return _ApplicationName;
}

static Version _ApplicationVersion;

TEMPORALITY_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch)
{
    _ApplicationVersion = Version(major, minor, patch);
}

TEMPORALITY_ENGINE_API
void SetApplicationVersion(const Version& version)
{
    _ApplicationVersion = version;
}

TEMPORALITY_ENGINE_API
Version GetApplicationVersion()
{
    return _ApplicationVersion;
}

} // namespace Temporality