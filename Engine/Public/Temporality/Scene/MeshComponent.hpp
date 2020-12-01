#ifndef TEMPORALITY_MESH_COMPONENT_HPP
#define TEMPORALITY_MESH_COMPONENT_HPP

#include <Temporality/Config.hpp>
#include <Temporality/Scene/Component.hpp>
#include <Temporality/Graphics/Mesh.hpp>

#include <memory>
#include <vector>

namespace Temporality {

class TEMPORALITY_ENGINE_API MeshComponent : public Component
{
public:
    DISALLOW_COPY_AND_ASSIGN(MeshComponent)

    MeshComponent() = default;

    virtual ~MeshComponent() = default;

    bool LoadFromFile(const std::string& filename);

    void AddMesh(std::shared_ptr<Mesh>&& mesh);
    
    void Render(RenderContext * ctx) override;

private:

    std::vector<std::shared_ptr<Mesh>> _mMeshes;

}; // class MeshComponent

} // namespace Temporality

#endif // TEMPORALITY_MESH_COMPONENT_HPP