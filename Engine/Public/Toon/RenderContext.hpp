#ifndef TOON_RENDER_CONTEXT_HPP
#define TOON_RENDER_CONTEXT_HPP

#include <Toon/Config.hpp>
#include <Toon/ShaderGlobals.hpp>
#include <Toon/ShaderTransform.hpp>

namespace Toon {

class TOON_ENGINE_API RenderContext
{
public:

    DISALLOW_COPY_AND_ASSIGN(RenderContext)

    RenderContext() = default;

    virtual ~RenderContext() = default;

    virtual ShaderGlobals * GetShaderGlobals() {
        return &_shaderGlobals;
    }

    virtual ShaderTransform* GetShaderTransform() {
        return &_shaderTransform;
    }

private:

    ShaderGlobals _shaderGlobals;

    ShaderTransform _shaderTransform;
    
}; // class RenderContext

} // namesapce Toon

#endif // TOON_RENDER_CONTEXT_HPP