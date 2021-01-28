#ifndef TOON_RENDER_CONTEXT_HPP
#define TOON_RENDER_CONTEXT_HPP

#include <Toon/Config.hpp>
#include <Toon/TransformData.hpp>

namespace Toon {

class RenderContext
{
public:

    DISALLOW_COPY_AND_ASSIGN(RenderContext)

    RenderContext() = default;

    virtual ~RenderContext() = default;

    virtual TransformData* GetTransformData() {
        return &_mTransformData;
    }

private:

    TransformData _mTransformData;
    
}; // class RenderContext

} // namesapce Toon

#endif // TOON_RENDER_CONTEXT_HPP