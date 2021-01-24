#ifndef TEMPORALITY_PRIMITIVE_DATA_HPP
#define TEMPORALITY_PRIMITIVE_DATA_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Primitive.hpp>

#include <gsl/gsl>

namespace Temporality {

class TEMPORALITY_ENGINE_API PrimitiveData
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

} // namespace Temporality

#endif // TEMPORALITY_PRIMITIVE_DATA_HPP