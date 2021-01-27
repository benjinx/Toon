#ifndef TEMPORALITY_SHADER_HPP
#define TEMPORALITY_SHADER_HPP

#include <Temporality/Config.hpp>

#include <string>
#include <vector>

namespace Temporality {

class TEMPORALITY_ENGINE_API Shader
{
public:
    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    virtual ~Shader() = default;

    virtual bool LoadFromFiles(const std::vector<std::string>& filenames) = 0;

}; // class Shader

} // namespace Temporality

#endif // TEMPORALITY_SHADER_HPP