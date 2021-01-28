#ifndef TOON_PRIMITIVE_DATA_HPP
#define TOON_PRIMITIVE_DATA_HPP

#include <Toon/Config.hpp>
#include <Toon/Primitive.hpp>

#include <gsl/gsl>

namespace Toon {

class TOON_ENGINE_API PrimitiveData
{
public:

    DISALLOW_COPY_AND_ASSIGN(PrimitiveData);

    PrimitiveData() = default;

    virtual ~PrimitiveData() = default;

    virtual PrimitiveTopology GetTopology() const = 0;

    virtual gsl::span<uint32_t> GetIndexList() {
        return gsl::span<uint32_t>();
    }
    
    virtual gsl::span<Vertex> GetVertexList() = 0;

    virtual void CalculateTBN();

}; // class PrimitiveData

} // namespace Toon

#endif // TOON_PRIMITIVE_DATA_HPP