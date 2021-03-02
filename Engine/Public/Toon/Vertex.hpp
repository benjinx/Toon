#ifndef TOON_VERTEX_HPP
#define TOON_VERTEX_HPP

#include <Toon/Config.hpp>
#include <Toon/Math.hpp>
#include <Toon/String.hpp>

namespace Toon {

struct Vertex
{
    glm::vec4 Position;
    glm::vec4 Normal;
    glm::vec4 Tangent;
    glm::vec4 Bitangent;
    
    glm::vec4 Color;

    glm::vec2 TexCoord1;
    glm::vec2 TexCoord2;

    uint16_t Joints[4];
    float Weights[4];
    
}; // struct Vertex

enum class VertexAttribute
{
    Position,
    Normal,
    Tangent,
    Bitangent,
    Color,
    TexCoord1,
    TexCoord2,
    Joints,
    Weights,

}; // enum class VertexAttribute

inline string VertexAttributeToString(VertexAttribute vertexAttribute)
{
    switch (vertexAttribute) {
        case VertexAttribute::Position:
            return "Position";
        case VertexAttribute::Normal:
            return "Normal";
        case VertexAttribute::Tangent:
            return "Tangent";
        case VertexAttribute::Bitangent:
            return "Bitangent";
        case VertexAttribute::Color:
            return "Color";
        case VertexAttribute::TexCoord1:
            return "TexCoord1";
        case VertexAttribute::TexCoord2:
            return "TexCoord2";
        case VertexAttribute::Joints:
            return "Joints";
        case VertexAttribute::Weights:
            return "Weights";
    }

    return "Unknown";
}

inline uint32_t GetVertexAttributeLocation(VertexAttribute attribute)
{
    switch (attribute) {
    case VertexAttribute::Position:
        return 0;
    case VertexAttribute::Normal:
        return 1;
    case VertexAttribute::Tangent:
        return 2;
    case VertexAttribute::Bitangent:
        return 3;
    case VertexAttribute::Color:
        return 4;
    case VertexAttribute::TexCoord1:
        return 5;
    case VertexAttribute::TexCoord2:
        return 6;
    case VertexAttribute::Joints:
        return 7;
    case VertexAttribute::Weights:
        return 8;
    }

    return UINT32_MAX;
}

} // namespace Toon

#endif // TOON_VERTEX_HPP
