#ifndef TEMPORALITY_OPENGL_SHADER_HPP
#define TEMPORALITY_OPENGL_SHADER_HPP

#include <Temporality/OpenGL/Config.hpp>
#include <Temporality/Graphics/Shader.hpp>

namespace Temporality::OpenGL {

class TEMPORALITY_OPENGL_API Shader : public Temporality::Shader 
{
public:
    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind() override;

    GLuint GetID();

private:
    GLuint LoadSPV(const std::string& filename);

    GLuint LoadGLSL(const std::string& filename);

    GLenum GetGLShaderType(const std::string& filename);

    GLuint _mglID = 0;

}; // class Shader

} // namespace Temporality::OpenGL
#endif // TEMPORALITY_OPENGL_SHADER_HPP