#include <Temporality/TinyOBJ/TinyOBJMeshImporter.hpp>
#include <Temporality/Log.hpp>
#include <Temporality/Utils.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Temporality::TinyOBJ {

TEMPORALITY_TINYOBJ_API
std::vector<std::unique_ptr<MeshData>> TinyOBJMeshImporter::LoadFromFile(const std::string& filename)
{
    BenchmarkStart();

    //const std::string& dir = GetDirname(filename);
    
    std::vector<std::unique_ptr<MeshData>> meshes;

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
    bool hasUVs = (!attrib.texcoords.empty());
    bool hasColors = (!attrib.colors.empty());

    for (auto& shape : shapes) {
        auto mesh = std::make_unique<TinyOBJMeshData>();

        mesh->Vertices.reserve(shape.mesh.indices.size());

        if (hasNormals) {
            mesh->Normals.reserve(shape.mesh.indices.size());
        }

        if (hasUVs) {
            mesh->UVs.reserve(shape.mesh.indices.size());
        }

        if (hasColors) {
            mesh->Colors.reserve(shape.mesh.indices.size());
        }

        for (const auto& i : shape.mesh.indices) {
            mesh->Vertices.push_back(attrib.vertices[3 * i.vertex_index + 0]);
            mesh->Vertices.push_back(attrib.vertices[3 * i.vertex_index + 1]);
            mesh->Vertices.push_back(attrib.vertices[3 * i.vertex_index + 2]);
            mesh->Vertices.push_back(1.0f);

            if (hasNormals) {
                mesh->Normals.push_back(attrib.normals[3 * i.normal_index + 0]);
                mesh->Normals.push_back(attrib.normals[3 * i.normal_index + 1]);
                mesh->Normals.push_back(attrib.normals[3 * i.normal_index + 2]);
                mesh->Normals.push_back(1.0f);
            }

            if (hasUVs) {
                mesh->UVs.push_back(attrib.texcoords[2 * i.texcoord_index + 0]);
                mesh->UVs.push_back(attrib.texcoords[2 * i.texcoord_index + 1]);
            }

            if (hasColors) {
                mesh->Colors.push_back(attrib.colors[3 * i.vertex_index + 0]);
                mesh->Colors.push_back(attrib.colors[3 * i.vertex_index + 1]);
                mesh->Colors.push_back(attrib.colors[3 * i.vertex_index + 2]);
                mesh->Colors.push_back(1.0f);
            }
        }

        meshes.push_back(std::move(mesh));
    }

    BenchmarkEnd("TinyOBJ::MeshImporter::LoadFromFile");
    return meshes;
}

} // namespace Temporality::TinyOBJ