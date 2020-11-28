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

} // namespace Temporality