#ifndef TOON_SHADER_HPP
#define TOON_SHADER_HPP

#include <Toon/Config.hpp>

#include <Toon/String.hpp>
#include <vector>

namespace Toon {

enum class ShaderStage
{
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessControl,
    TessEvaluation,

}; // enum class ShaderStage

class TOON_ENGINE_API Shader
{
public:
    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    virtual ~Shader() = default;

    virtual void Terminate() { }

    virtual bool LoadFromFiles(const std::vector<string>& filenames, bool useAssetPath = true) = 0;

}; // class Shader

inline string ShaderStageToString(ShaderStage shaderStage)
{
    switch (shaderStage) {
        case ShaderStage::Vertex:
            return "Vertex";
        case ShaderStage::Fragment:
            return "Fragment";
        case ShaderStage::Geometry:
            return "Geometry";
        case ShaderStage::Compute:
            return "Compute";
        case ShaderStage::TessControl:
            return "TessControl";
        case ShaderStage::TessEvaluation:
            return "TessEvaluation";
    }

    return "Unknown";
}

} // namespace Toon

#endif // TOON_SHADER_HPP