#include <Toon/OpenGL/OpenGLMaterial.hpp>
#include <Toon/OpenGL/OpenGLBuffer.hpp>
#include <Toon/OpenGL/OpenGLTexture.hpp>
#include <Toon/GraphicsDriver.hpp>

namespace Toon::OpenGL {

void OpenGLMaterial::Bind()
{
    auto gfx = GetGraphicsDriver();

    OpenGLBuffer * glMaterialBuffer = TOON_OPENGL_BUFFER(_shaderMaterialBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, TOON_SHADER_MATERIAL_BINDING, glMaterialBuffer->GetGLID());

    OpenGLTexture * glDefaultTexture = TOON_OPENGL_TEXTURE(gfx->GetDefaultTexture().get());
    GLuint defaultTextureID = (glDefaultTexture ? glDefaultTexture->GetGLID() : 0);

    OpenGLTexture * glTexture = nullptr;

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::BaseColorMap));
    glTexture = TOON_OPENGL_TEXTURE(_baseColorMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::BaseColorMap));
    glTexture = TOON_OPENGL_TEXTURE(_baseColorMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::NormalMap));
    glTexture = TOON_OPENGL_TEXTURE(_normalMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::MetallicRoughnessMap));
    glTexture = TOON_OPENGL_TEXTURE(_metallicRoughnessMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::EmissiveMap));
    glTexture = TOON_OPENGL_TEXTURE(_emissiveMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::OcclusionMap));
    glTexture = TOON_OPENGL_TEXTURE(_occlusionMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));
}

} // namespace Toon::OpenGL