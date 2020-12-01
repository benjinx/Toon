#ifndef TEMPORALITY_RENDER_CONTEXT_HPP
#define TEMPORALITY_RENDER_CONTEXT_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Graphics/TransformData.hpp>

namespace Temporality {

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

} // namesapce Temporality

#endif // TEMPORALITY_RENDER_CONTEXT_HPP