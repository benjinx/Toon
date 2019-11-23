#include "Model.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Texture.hpp>
#include <Utils.hpp>

#include <nlohmann/json.hpp>

#include <stb/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

Model::Model(std::vector<Mesh*> meshes)
{
    _mMeshes = meshes;
}

Model::~Model()
{
    for (Mesh* mesh : _mMeshes)
    {
        delete mesh;
    }
}

void Model::Render(Shader* shader, glm::mat4 modelMatrix)
{
    for (Mesh* mesh : _mMeshes)
    {
        mesh->Render(shader, modelMatrix);
    }
}