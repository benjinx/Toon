#ifndef TEMPORALITY_MESH_HPP
#define TEMPORALITY_MESH_HPP

#include <Temporality/Config.hpp>

#include <gsl/gsl>

namespace Temporality {

class TEMPORALITY_ENGINE_API MeshData
{
public:
    enum class Mode
    {
        Points,
        Lines,
        Triangles,
        TriangleFan,
    }; // enum class Mode

    DISALLOW_COPY_AND_ASSIGN(MeshData)

    MeshData() = default;

    virtual ~MeshData() = default;

    virtual Mode GetMode() const = 0;

    virtual gsl::span<const unsigned int> GetIndices() const = 0;

    virtual gsl::span<const float> GetVertices() const = 0;

    virtual gsl::span<const float> GetNormals() const = 0;

    virtual gsl::span<const float> GetUVs() const = 0;

    virtual gsl::span<const float> GetColors() const = 0;

    virtual gsl::span<const float> GetTangents() const = 0;

    virtual gsl::span<const float> GetBitangets() const = 0;

    virtual gsl::span<const unsigned short> GetJoints() const = 0;

    virtual gsl::span<const float> GetWeights() const = 0;

}; // class MeshData

class TEMPORALITY_ENGINE_API Mesh
{
public:
    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    virtual ~Mesh() = default;

    virtual void Render() = 0;

    virtual bool Load(const MeshData* data) = 0;

protected:
    enum class VertexAttributeLocation {
        Position        = 0,
        Normal          = 1,
        Tangent         = 2,
        Bitangent       = 3,
        Joints          = 4,
        Weights         = 5,
        Color1          = 6,
        Color2          = 7,
        UV1             = 8,
        UV2             = 8,
        UV3             = 9,
        UV4             = 9,
        UV5             = 10,
        UV6             = 10,
    };
}; // class Mesh

} // namespace Temporality

#endif // TEMPORALITY_MESH_HPP