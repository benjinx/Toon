#ifndef MESH_H
#define MESH_H

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <memory>
#include <vector>

class Material;
class Shader;

class Mesh
{
public:

    struct Primitive
    {
        GLuint      VAO;
        GLenum      Mode;
        GLsizei     Count;
        GLenum      Type;
        GLsizei     Offset;

        std::shared_ptr<Material> _Material;
    };


    enum AttributeID : GLint
    {
        POSITION = 0,
        NORMAL = 1,
        TEXCOORD = 2,
        TANGENT = 3,
        BITANGENT = 4,
        COLOR = 5,
    };

    Mesh() = default;

    /* Functions */
    Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material);
    Mesh(std::vector<Primitive>&&);

    bool LoadFromData(std::vector<Primitive>&& primitives);

    void Render(Shader * shader, glm::mat4 modelMat);

private:
    std::vector<Primitive> _mPrimitives;
};

#endif // MESH_H
