#ifndef TEMPORALITY_TINYOBJ_MESH_IMPORTER_HPP
#define TEMPORALITY_TINYOBJ_MESH_IMPORTER_HPP

#include <Temporality/TinyOBJ/Config.hpp>
#include <Temporality/Graphics/MeshImporter.hpp>

#include <vector>

namespace Temporality::TinyOBJ {

class TEMPORALITY_TINYOBJ_API MeshData : public Temporality::MeshData
{
public:

    std::vector<float> Vertices;

    std::vector<float> Normals;

    std::vector<float> UVs;

    std::vector<float> Colors;

    Mode GetMode() const override {
        return Mode::Triangles;
    }

    gsl::span<const unsigned int> GetIndices() const override {
        return gsl::span<unsigned int>();
    }

    gsl::span<const float> GetVertices() const override {
        return gsl::span<const float>(Vertices.data(), Vertices.size());
    }

    gsl::span<const float> GetNormals() const override {
        return gsl::span<const float>(Normals.data(), Normals.size());
    }

    gsl::span<const float> GetUVs() const override {
        return gsl::span<const float>(UVs.data(), UVs.size());
    }

    gsl::span<const float> GetColors() const override {
        return gsl::span<const float>(Colors.data(), Colors.size());
    }

    gsl::span<const float> GetTangents() const override {
        return gsl::span<float>();
        // We should have a function to calculate tangents
    }

    gsl::span<const float> GetBitangets() const override {
        return gsl::span<float>();
        // We should have a function to calculate bitangents
    }

}; // class MeshData

class TEMPORALITY_TINYOBJ_API MeshImporter : public Temporality::MeshImporter
{
public:

    std::vector<std::unique_ptr<Temporality::MeshData>> LoadFromFile(const std::string& filename) override;
    
}; // class MeshImporter

} // namespace Temporality::TinyOBJ

#endif // TEMPORALITY_TINYOBJ_MESH_IMPORTER_HPP