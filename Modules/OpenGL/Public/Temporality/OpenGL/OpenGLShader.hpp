#ifndef TEMPORALITY_OPENGL_SHADER_HPP
#define TEMPORALITY_OPENGL_SHADER_HPP

#include <Temporality/OpenGL/OpenGLConfig.hpp>
#include <Temporality/Shader.hpp>

namespace Temporality::OpenGL {

#define TEMPORALITY_OPENGL_SHADER(x) (dynamic_cast<Temporality::OpenGL::OpenGLShader *>(x))

class TEMPORALITY_OPENGL_API OpenGLShader : public Shader 
{
public:
    DISALLOW_COPY_AND_ASSIGN(OpenGLShader)

    OpenGLShader() = default;

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind();

    GLuint GetID();

private:
    GLuint LoadSPV(const std::string& filename);

    GLuint LoadGLSL(const std::string& filename);

    GLenum GetGLShaderType(const std::string& filename);

    GLuint _mglID = 0;

}; // class OpenGLShader

} // namespace Temporality::OpenGL

#endif // TEMPORALITY_OPENGL_SHADER_HPP