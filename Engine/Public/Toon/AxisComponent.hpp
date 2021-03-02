#ifndef TOON_AXIS_COMPONENT_HPP
#define TOON_AXIS_COMPONENT_HPP

#include <Toon/Config.hpp>
#include <Toon/Component.hpp>
#include <Toon/Mesh.hpp>
#include <Toon/Shader.hpp>

namespace Toon {

// class TOON_ENGINE_API AxisPrimitiveData : public PrimitiveData
// {
// public:

//     const unsigned Indices[6] = {
//         0, 1, 
//         2, 3, 
//         4, 5,
//     };

//     const float Vertices[24] = { 
//         0.f, 0.f, 0.f, 1.0f,
//         1.f, 0.f, 0.f, 1.0f,
//         0.f, 0.f, 0.f, 1.0f,
//         0.f, 1.f, 0.f, 1.0f,
//         0.f, 0.f, 0.f, 1.0f,
//         0.f, 0.f, 1.f, 1.0f,
//     };

//     const float Colors[24] = { 
//         1.f, 0.f, 0.f, 1.0f,
//         1.f, 0.f, 0.f, 1.0f,
//         0.f, 1.f, 0.f, 1.0f,
//         0.f, 1.f, 0.f, 1.0f,
//         0.f, 0.f, 1.f, 1.0f,
//         0.f, 0.f, 1.f, 1.0f,
//     };

//     Mode GetMode() const override {
//         return Mode::Lines;
//     }

//     gsl::span<const unsigned> GetIndices() const override {
//         return gsl::span(Indices, 6);
//     }

//     gsl::span<const float> GetVertices() const override {
//         return gsl::span(Vertices, 24);
//     }

//     gsl::span<const float> GetNormals() const override {
//         return gsl::span<float>();
//     }

//     gsl::span<const float> GetUVs() const override {
//         return gsl::span<float>();
//     }

//     gsl::span<const float> GetColors() const override {
//         return gsl::span(Colors, 24);
//     }

//     gsl::span<const float> GetTangents() const override {
//         return gsl::span<float>();
//     }

//     gsl::span<const float> GetBitangents() const override {
//         return gsl::span<float>();
//     }

// }; // class PrimitiveData

class TOON_ENGINE_API AxisComponent : public Component
{
public:

    DISALLOW_COPY_AND_ASSIGN(AxisComponent)

    AxisComponent();

    virtual ~AxisComponent() = default;

    void Render(RenderContext * ctx) override;
private:
    //static AxisPrimitiveData _PrimitiveData;

    std::shared_ptr<Shader> _shader;

    std::shared_ptr<Mesh> _mesh;

}; // class AxisComponent


} // namespace Toon

#endif // TOON_AXIS_COMPONENT_HPP