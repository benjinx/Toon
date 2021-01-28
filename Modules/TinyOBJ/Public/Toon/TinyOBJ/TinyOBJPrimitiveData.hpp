#ifndef TOON_TINYOBJ_PRIMITIVE_DATA_HPP
#define TOON_TINYOBJ_PRIMITIVE_DATA_HPP

#include <Toon/TinyOBJ/TinyOBJConfig.hpp>

#include <Toon/PrimitiveData.hpp>

namespace Toon::TinyOBJ {

class TOON_TINYOBJ_API TinyOBJPrimitiveData : public PrimitiveData
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

} // namespace Toon::TinyOBJ

#endif // TOON_TINYOBJ_PRIMITIVE_DATA_HPP