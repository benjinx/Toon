#ifndef TOON_SHADER_HPP
#define TOON_SHADER_HPP

#include <Toon/Config.hpp>

#include <string>
#include <vector>

namespace Toon {

class TOON_ENGINE_API Shader
{
public:
    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    virtual ~Shader() = default;

    virtual bool LoadFromFiles(const std::vector<std::string>& filenames) = 0;

}; // class Shader

} // namespace Toon

#endif // TOON_SHADER_HPP