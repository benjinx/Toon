#ifndef TOON_GLTF2_PRIMITIVE_DATA_HPP
#define TOON_GLTF2_PRIMITIVE_DATA_HPP

#include <Toon/PrimitiveData.hpp>

namespace Toon::GLTF2 {

class TOON_GLTF2_API GLTF2PrimitiveData : public PrimitiveData
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2PrimitiveData)

    GLTF2PrimitiveData() = default;

    virtual ~GLTF2PrimitiveData() = default;

    std::vector<uint32_t> IndexList;

    std::vector<Vertex> VertexList;

    PrimitiveTopology GetTopology() const override {
        return PrimitiveTopology::TriangleList;
    }

    gsl::span<uint32_t> GetIndexList() override {
        return IndexList;
    }

    gsl::span<Vertex> GetVertexList() override {
        return VertexList;
    }

}; // class GLTF2PrimitiveData

} // namespace Toon::GLTF2

#endif // TOON_GLTF2_PRIMITIVE_DATA_HPP