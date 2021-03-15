#include <Toon/TinyOBJ/TinyOBJMeshImporter.hpp>
#include <Toon/Log.hpp>
#include <Toon/Benchmark.hpp>
#include <Toon/Util.hpp>
#include <Toon/TinyOBJ/TinyOBJPrimitiveData.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Toon::TinyOBJ {

TOON_TINYOBJ_API
std::vector<std::unique_ptr<PrimitiveData>> TinyOBJMeshImporter::LoadFromFile(const std::string& filename, bool useAssetPath /*= true*/)
{
    ToonBenchmarkStart();

    std::vector<std::unique_ptr<PrimitiveData>> primitiveList;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapeList;
    std::vector<tinyobj::material_t> materialList;

    std::string warn, err;
    bool result = false;

    if (useAssetPath) {
        const auto& assetPaths = GetAssetPathList();

        for (const auto& path : assetPaths) {
            std::string fullPath = path / "Models" / filename;
            ToonLogInfo("Loading from File: %s", fullPath);

            std::string dir = GetDirname(fullPath);

            warn = "";
            err = "";

            result = tinyobj::LoadObj(&attrib, &shapeList, &materialList, &warn, &err, fullPath.c_str(), dir.c_str());

            // If the error isn't 'Cannot open file', the .obj file is probably
            // broken, and we should fail
            if (!err.empty() && err.rfind("Cannot open file", 0) != 0) {
                break;
            }

            if (result) {
                break;
            }
        }
    }
    else {
        string dir = GetDirname(filename);
        result = tinyobj::LoadObj(&attrib, &shapeList, &materialList, &warn, &err, filename.c_str(), dir.c_str());
    }


    if (!warn.empty()) {
        ToonLogWarn("tinyobj: %s", warn);
    }

    if (!err.empty()) {
        ToonLogError("tinyobj: %s", err);
    }

    bool hasNormals = (!attrib.normals.empty());
    bool hasTexCoords = (!attrib.texcoords.empty());
    bool hasColors = (!attrib.colors.empty());

    for (auto& shape : shapeList) {
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

        primitiveData->CalculateTangents();
        primitiveList.push_back(std::unique_ptr<PrimitiveData>(primitiveData));
    }

    ToonBenchmarkEnd();
    return primitiveList;
}

} // namespace Toon::TinyOBJ