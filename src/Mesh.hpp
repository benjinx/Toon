#ifndef MESH_H
#define MESH_H

#include "Config.hpp"
#include "Light.hpp"

class Shader;
class Material;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh
{
public:

    enum Attrib {
        POSITION = 0,
        NORMAL = 2,
        COLOR = 3,
        TEXCOORD = 8,
        TANGENT = 10,
        BITANGENT = 11,
    };

    /* Functions */
    Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents);

	void Render(Shader * shader, glm::mat4 modelMat);

    void SetMaterial(Material* material) { _mMaterial = material; }

    GLuint GetVAO() { return _mVAO; }

private:

	Material * _mMaterial = nullptr;

    /* Render Data */
    GLuint       _mVAO, _mVBOs[5];
    unsigned int _mVertCount;

};

#endif // MESH_H
