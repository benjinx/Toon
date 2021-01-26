#ifndef TEMPORALITY_MESH_HPP
#define TEMPORALITY_MESH_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Primitive.hpp>
#include <Temporality/PrimitiveData.hpp>
#include <Temporality/Pipeline.hpp>
#include <Temporality/RenderContext.hpp>

#include <vector>
#include <memory>

namespace Temporality {

class TEMPORALITY_ENGINE_API Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    virtual ~Mesh() = default;

    virtual bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data);

    virtual void Render(RenderContext * ctx) = 0;

    virtual void SetPipeline(std::shared_ptr<Pipeline> pipeline) {
        _pipeline = pipeline;
    }

protected:

    std::vector<std::unique_ptr<Primitive>> _primitiveList;

    std::shared_ptr<Pipeline> _pipeline;

}; // class Mesh

TEMPORALITY_ENGINE_API
std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename);

} // namespace Temporality

#endif // TEMPORALITY_MESH_HPP