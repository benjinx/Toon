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

Model::Model(std::vector<std::unique_ptr<Mesh>> && meshes)
{
    _mMeshes = std::move(meshes);
}

void Model::Render(Shader* shader, glm::mat4 modelMatrix)
{
    for (auto& mesh : _mMeshes)
    {
        mesh->Render(shader, modelMatrix);
    }
}