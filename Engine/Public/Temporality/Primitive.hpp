#ifndef TEMPORALITY_PRIMITIVE_HPP
#define TEMPORALITY_PRIMITIVE_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Math.hpp>
#include <Temporality/Vertex.hpp>

#include <memory>

namespace Temporality {

enum class PrimitiveTopology 
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,

}; // enum class PrimitiveTopology

class PrimitiveData;

class TEMPORALITY_ENGINE_API Primitive
{
public:

    DISALLOW_COPY_AND_ASSIGN(Primitive)

    Primitive() = default;

    virtual ~Primitive() = default;

    virtual bool Load(const std::unique_ptr<PrimitiveData>& data) = 0;

}; // class Primitive

inline std::string PrimitiveTopologyToString(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::Points:
        return "Points";
    case PrimitiveTopology::Lines:
        return "Lines";
    case PrimitiveTopology::LineStrip:
        return "LineStrip";
    case PrimitiveTopology::Triangles:
        return "Triangles";
    case PrimitiveTopology::TriangleStrip:
        return "TriangleStrip";
    }

    return "Unknown";
}

} // namespace Temporality

#endif // TEMPORALITY_PRIMITIVE_HPP