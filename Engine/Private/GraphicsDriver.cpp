#include <Temporality/Graphics/GraphicsDriver.hpp>

namespace Temporality {

static std::unique_ptr<GraphicsDriver> _gGraphicsDriver(nullptr);

void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver)
{
    _gGraphicsDriver = std::move(driver);    
}

GraphicsDriver* GetGraphicsDriver()
{
    return _gGraphicsDriver.get();
}

}