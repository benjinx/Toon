#ifndef TOON_PRIMITIVE_HPP
#define TOON_PRIMITIVE_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/Vertex.hpp>

#include <memory>

namespace Toon {

enum class PrimitiveTopology 
{
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,

}; // enum class PrimitiveTopology

class PrimitiveData;

class TOON_ENGINE_API Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(Primitive)

    Primitive() = default;

    virtual ~Primitive() = default;

    virtual void Terminate() { }

    virtual bool Load(const std::unique_ptr<PrimitiveData>& data) = 0;

}; // class Primitive

inline string PrimitiveTopologyToString(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::PointList:
        return "Points";
    case PrimitiveTopology::LineList:
        return "Lines";
    case PrimitiveTopology::LineStrip:
        return "LineStrip";
    case PrimitiveTopology::TriangleList:
        return "TriangleList";
    case PrimitiveTopology::TriangleStrip:
        return "TriangleStrip";
    }

    return "Unknown";
}

} // namespace Toon

#endif // TOON_PRIMITIVE_HPP