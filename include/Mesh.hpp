#ifndef MESH_H
#define MESH_H

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <memory>

class Material;
class Shader;

class Mesh
{
public:
    Mesh() = default;

    /* Functions */
    Mesh(GLuint vao, GLenum mode, GLsizei count, std::shared_ptr<Material> material);

    void Render(Shader * shader, glm::mat4 modelMat);

    GLuint GetVAO() { return _mVAO; }

private:
    /* Render Data */
    GLuint      _mVAO;
    GLenum      _mMode;
    GLsizei     _mCount;
    
    std::shared_ptr<Material> _mMaterial = nullptr;
};

#endif // MESH_H
