#ifndef TOON_MESH_HPP
#define TOON_MESH_HPP

#include <Toon/Config.hpp>
#include <Toon/Primitive.hpp>
#include <Toon/PrimitiveData.hpp>
#include <Toon/Pipeline.hpp>
#include <Toon/RenderContext.hpp>
#include <Toon/Texture.hpp>

#include <vector>
#include <memory>

namespace Toon {

class TOON_ENGINE_API Mesh
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

    std::shared_ptr<Texture> _texture;

protected:

    std::vector<std::unique_ptr<Primitive>> _primitiveList;

    std::shared_ptr<Pipeline> _pipeline;

}; // class Mesh

TOON_ENGINE_API
std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename);

} // namespace Toon

#endif // TOON_MESH_HPP