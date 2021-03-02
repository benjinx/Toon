#include <Toon/InputDriver.hpp>

namespace Toon {

static std::unique_ptr<InputDriver> _InputDriver(nullptr);

TOON_ENGINE_API
void SetInputDriver(std::unique_ptr<InputDriver> && driver)
{
    _InputDriver = std::move(driver);
}

TOON_ENGINE_API
InputDriver * GetInputDriver()
{
    return _InputDriver.get();
}

} // namespace Toon
