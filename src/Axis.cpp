#include "Axis.hpp"

#include <App.hpp>
#include <MeshComponent.hpp>
#include <Shader.hpp>

Axis::Axis()
{
    if (!_mMesh)
    {
        GLuint vao;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLfloat position[] = {
            0.0f, 0.0f, 0.0f,
            0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f,
        };

        GLfloat color[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
        };

        GLuint vbos[3];
        glGenBuffers(2, vbos);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
        glVertexAttribPointer(Mesh::AttributeID::POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(Mesh::AttributeID::POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        glVertexAttribPointer(Mesh::AttributeID::COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(Mesh::AttributeID::COLOR);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDeleteBuffers(2, vbos);

        GLsizei count = sizeof(position) / sizeof(position[0]);

        _mMesh = std::make_shared<Mesh>(
            vao,
            (GLenum)GL_LINES,
            (GLsizei)count,
            GL_UNSIGNED_INT,
            0,
            nullptr
        );
    }
}

void Axis::Render(glm::mat4 transform)
{
    Shader* shader = App::Inst()->GetShader("axis");
    glClear(GL_DEPTH_BUFFER_BIT);
    _mMesh->Render(shader, transform);
}