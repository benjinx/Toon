#ifndef TOON_OPENGL_SHADER_HPP
#define TOON_OPENGL_SHADER_HPP

#include <Toon/OpenGL/OpenGLConfig.hpp>
#include <Toon/Shader.hpp>

namespace Toon::OpenGL {

#define TOON_OPENGL_SHADER(x) (dynamic_cast<Toon::OpenGL::OpenGLShader *>(x))

class TOON_OPENGL_API OpenGLShader : public Shader 
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

} // namespace Toon::OpenGL

#endif // TOON_OPENGL_SHADER_HPP