#ifndef TOON_MESH_COMPONENT_HPP
#define TOON_MESH_COMPONENT_HPP

#include <Toon/Config.hpp>
#include <Toon/Buffer.hpp>
#include <Toon/Component.hpp>
#include <Toon/Mesh.hpp>

#include <memory>
#include <vector>

namespace Toon {

class TOON_ENGINE_API MeshComponent : public Component
{
public:
    DISALLOW_COPY_AND_ASSIGN(MeshComponent)

    MeshComponent() = default;

    MeshComponent(std::shared_ptr<Mesh> mesh);

    virtual ~MeshComponent() = default;

    void SetMesh(std::shared_ptr<Mesh> mesh);
    
    void Render(RenderContext * ctx) override;

private:

    std::shared_ptr<Mesh> _mesh;

}; // class MeshComponent

} // namespace Toon

#endif // TOON_MESH_COMPONENT_HPP