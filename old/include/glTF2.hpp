#ifndef GLTF2_HPP
#define GLTF2_HPP

#include <Entity.hpp>
#include <Mesh.hpp>

#include <string>
#include <vector>
#include <memory>

namespace glTF2 {

    std::vector<std::unique_ptr<Entity>> LoadSceneFromFile(const std::string& filename);

    std::vector<Mesh::Primitive> LoadPrimitivesFromFile(const std::string& filename);

} // namespace glTF2

#endif // GLTF2_HPP