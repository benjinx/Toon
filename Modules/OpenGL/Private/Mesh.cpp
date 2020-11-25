#include <Temporality/OpenGL/Mesh.hpp>

#include <Temporality/Log.hpp>

namespace Temporality::OpenGL {

TEMPORALITY_OPENGL_API
void Mesh::Render()
{
    glBindVertexArray(_mglVAO);

    if (_mIndexed) {
        glDrawElements(_mglMode, _mglCount, GL_UNSIGNED_INT, NULL);
    }
    else {
        glDrawArrays(_mglMode, 0, _mglCount);
    }

    glBindVertexArray(0);
}

TEMPORALITY_OPENGL_API
bool Mesh::Load(const MeshData* data)
{
    BenchmarkStart();

    glGenVertexArrays(1, &_mglVAO);
    glBindVertexArray(_mglVAO);

    const auto& indices = data->GetIndices();
    const auto& vertices = data->GetVertices();
    const auto& normals = data->GetNormals();
    const auto& uvs = data->GetUVs();
    const auto& colors = data->GetColors();
    const auto& joints = data->GetJoints();
    const auto& weights = data->GetWeights();

    _mglMode = GetGLMode(data->GetMode());

    GLuint vbo;
    std::vector<GLuint> vbos;

    if (indices.empty()) {
        _mIndexed = false;
        _mglCount = vertices.size();
    }
    else {
        _mIndexed = true;
        _mglCount = indices.size();

        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

        vbos.push_back(vbo);
    }

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Position);
    glVertexAttribPointer((GLuint)VertexAttributeLocation::Position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    vbos.push_back(vbo);

    if (!normals.empty()) {
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Normal);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::Normal, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }

    if (!colors.empty()) {
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Color1);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::Color1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }

    if (!uvs.empty()) {
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::UV1);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::UV1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }

    if (!joints.empty()) {
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, joints.size() * sizeof(unsigned short), joints.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Joints);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::Joints, 4, GL_UNSIGNED_SHORT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }

    if (!weights.empty()) {
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(float), weights.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Weights);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::Weights, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }

    glBindVertexArray(0);

    BenchmarkEnd("OpenGL::Mesh::Load");
    return true;
}

TEMPORALITY_OPENGL_API
GLenum Mesh::GetGLMode(const MeshData::Mode& mode)
{
    switch (mode) {
    case MeshData::Mode::Points:
        return GL_POINTS;
    case MeshData::Mode::Lines:
        return GL_LINES;
    case MeshData::Mode::Triangles:
        return GL_TRIANGLES;
    case MeshData::Mode::TriangleFan:
        return GL_TRIANGLE_FAN;
    }

    return GL_INVALID_ENUM;
}

} // namespace Temporality::OpenGL