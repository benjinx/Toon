#include <Toon/PrimitiveData.hpp>

#include <Toon/Log.hpp>

namespace Toon {

void PrimitiveData::CalculateTangents()
{
    auto topology = GetTopology();
    auto indexList = GetIndexList();
    auto vertexList = GetVertexList();

    auto calcVertex = [](Vertex& v1, Vertex& v2, Vertex& v3)
    {
        glm::vec3 v = glm::vec3(v2.Position) - glm::vec3(v1.Position);
        glm::vec3 w = glm::vec3(v3.Position) - glm::vec3(v1.Position);
        glm::vec2 s = v2.TexCoord1 - v1.TexCoord1;
        glm::vec2 t = v3.TexCoord1 - v1.TexCoord1;

        float dir = 1.0f;
        if ((t.x * s.y - t.y * s.x) < 0.0f) {
            dir = -1.0f;
        }

        glm::vec3 tangent = {
        dir * (w.x * s.y - v.x * t.y),
        dir * (w.y * s.y - v.y * t.y),
        dir * (w.z * s.y - v.z * t.y),
        };

        v1.Tangent = glm::vec4(tangent - glm::vec3(v1.Normal) * (tangent * glm::vec3(v1.Normal)), 1.0f);
        v2.Tangent = glm::vec4(tangent - glm::vec3(v2.Normal) * (tangent * glm::vec3(v2.Normal)), 1.0f);
        v3.Tangent = glm::vec4(tangent - glm::vec3(v3.Normal) * (tangent * glm::vec3(v3.Normal)), 1.0f);

        v1.Tangent = glm::normalize(v1.Tangent);
        v2.Tangent = glm::normalize(v2.Tangent);
        v3.Tangent = glm::normalize(v3.Tangent);

        // TODO: Account for NaN and smoothing
    };

    if (topology == PrimitiveTopology::TriangleList) {
        if (indexList.empty()) {
            for (size_t i = 0; i < vertexList.size(); i += 3) {
                calcVertex(
                    vertexList[i + 0],
                    vertexList[i + 1],
                    vertexList[i + 2]
                );
            }
        }
        else {
            for (size_t i = 0; i < indexList.size(); i += 3) {
                calcVertex(
                    vertexList[indexList[i + 0]],
                    vertexList[indexList[i + 1]],
                    vertexList[indexList[i + 2]]
                );
            }
        }
    }
    else if (topology == PrimitiveTopology::TriangleStrip) {
        if (indexList.empty()) {
            for (size_t i = 0; i < vertexList.size(); ++i) {
                calcVertex(
                    vertexList[i + 0],
                    vertexList[i + 1],
                    vertexList[i + 2]
                );
            }
        }
        else {
            for (size_t i = 0; i < indexList.size(); ++i) {
                calcVertex(
                    vertexList[indexList[i + 0]],
                    vertexList[indexList[i + 1]],
                    vertexList[indexList[i + 2]]
                );
            }
        }
    }
}

} // namespace Toon