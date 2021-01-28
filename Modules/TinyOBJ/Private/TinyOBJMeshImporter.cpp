#include <Toon/TinyOBJ/TinyOBJMeshImporter.hpp>
#include <Toon/Log.hpp>
#include <Toon/Utils.hpp>
#include <Toon/TinyOBJ/TinyOBJPrimitiveData.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Toon::TinyOBJ {

TOON_TINYOBJ_API
std::vector<std::unique_ptr<PrimitiveData>> TinyOBJMeshImporter::LoadFromFile(const std::string& filename)
{
    BenchmarkStart();

    //const std::string& dir = GetDirname(filename);

    std::vector<std::unique_ptr<PrimitiveData>> primitiveList;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;
    bool result = false;

    const auto& assetPaths = GetAssetPaths();

    for (const auto& path : assetPaths)
    {
        std::string fullPath = path + filename;
        LogInfo("Loading from File: %s", fullPath);

        std::string dir = GetDirname(fullPath);

        warn = "";
        err = "";

        result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, fullPath.c_str(), dir.c_str());

        if (result) {
            break;
        }
    }


    if (!warn.empty()) {
        LogWarn("tinyobj: %s", warn);
    }

    if (!err.empty()) {
        LogError("tinyobj: %s", err);
    }

    bool hasNormals = (!attrib.normals.empty());
    bool hasTexCoords = (!attrib.texcoords.empty());
    bool hasColors = (!attrib.colors.empty());

    for (auto& shape : shapes) {
        TinyOBJPrimitiveData * primitiveData = new TinyOBJPrimitiveData();
        
        primitiveData->VertexList.resize(shape.mesh.indices.size());

        for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            auto index = shape.mesh.indices[i];
            auto& vertex = primitiveData->VertexList[i];

            vertex.Position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
                1.0f,
            };

            if (hasNormals) {
                vertex.Normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                    1.0f,
                };
            }
            
            if (hasColors) {
                vertex.Color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                    1.0f,
                };
            }
            
            if (hasTexCoords) {
                vertex.TexCoord1 = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }
        }

        primitiveData->CalculateTBN();
        primitiveList.push_back(std::unique_ptr<PrimitiveData>(primitiveData));
    }

    BenchmarkEnd("TinyOBJ::MeshImporter::LoadFromFile");
    return primitiveList;
}

} // namespace Toon::TinyOBJ