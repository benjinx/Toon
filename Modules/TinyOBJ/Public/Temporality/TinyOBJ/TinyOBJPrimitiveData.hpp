#ifndef TEMPORALITY_TINYOBJ_PRIMITIVE_DATA_HPP
#define TEMPORALITY_TINYOBJ_PRIMITIVE_DATA_HPP

#include <Temporality/TinyOBJ/TinyOBJConfig.hpp>

#include <Temporality/PrimitiveData.hpp>

namespace Temporality::TinyOBJ {

class TEMPORALITY_TINYOBJ_API TinyOBJPrimitiveData : public PrimitiveData
{
public:

    std::vector<Vertex> VertexList;

    PrimitiveTopology GetTopology() const override {
        return PrimitiveTopology::TriangleList;
    }

    gsl::span<Vertex> GetVertexList() override {
        return gsl::span<Vertex>(VertexList.data(), VertexList.size());
    }

}; // class TinyOBJPrimitiveData

} // namespace Temporality::TinyOBJ

#endif // TEMPORALITY_TINYOBJ_PRIMITIVE_DATA_HPP