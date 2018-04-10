#include "Mesh.hpp"

#include "Camera.hpp"
#include "Shader.hpp"
#include "Material.hpp"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normal, std::vector<glm::vec2> texCoords, std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents)
{
    // VBO, VAO.
    glGenVertexArrays(1, &_mVAO);
    glBindVertexArray(_mVAO);

    GLuint vbos[5];
    glGenBuffers(5, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_POSITION);

    if (!normal.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normal.size() * 3, normal.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(ATTRIB_NORMAL);
    }

    if (!texCoords.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    }

	if (!tangents.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size() * 3, tangents.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ATTRIB_TANGENT);
	}

	if (!bitangents.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbos[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bitangents.size() * 3, bitangents.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(ATTRIB_BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ATTRIB_BITANGENT);
	}

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _mVertCount = vertices.size();
}

void Mesh::Render(Shader * shader, glm::mat4 modelMat)
{
	shader->Use();

    const auto& view = Camera::instance().GetViewMat();
    const auto& proj = Camera::instance().GetProjectionMat();

	shader->SetMat4("modelMat", modelMat);
	shader->SetMat4("viewMat", view);
	shader->SetMat4("projMat", proj);

	glm::mat4 mvp = proj * view * modelMat;
	shader->SetMat4("mvp", mvp);

    glm::vec3 camPos = Camera::instance().GetCameraPos();
	glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
	shader->SetVec4("eyePos", eyePos);

	// Material values
	auto ambient = _mMaterial->GetAmbient();
	shader->SetVec3("ambient", ambient);

	auto diffuse = _mMaterial->GetDiffuse();
	shader->SetVec3("diffuse", diffuse);

	auto specular = _mMaterial->GetSpecular();
	shader->SetVec3("specular", specular);

	auto shininess = _mMaterial->GetShininess();
	shader->SetFloat("shininess", shininess);

	_mMaterial->Bind();

    if (_mMaterial->AmbientTexExists())
    {
		shader->SetInt("ambientTex", TextureID::AMBIENT);
		shader->SetBool("hasAmbient", true);
    }
	else
	{
		shader->SetBool("hasAmbient", false);
	}

    if (_mMaterial->DiffuseTexExists())
    {
		shader->SetInt("diffuseTex", TextureID::DIFFUSE);
		shader->SetBool("hasDiffuse", true);
    }
	else
	{
		shader->SetBool("hasDiffuse", false);
	}

    if (_mMaterial->SpecularTexExists())
    {
		shader->SetInt("specularTex", TextureID::SPECULAR);
		shader->SetBool("hasSpecular", true);
	}
	else
	{
		shader->SetBool("hasSpecular", false);
	}

    if (_mMaterial->NormalTexExists())
    {
		shader->SetInt("normalTex", TextureID::NORMAL);
		shader->SetBool("hasNormal", true);
	}
	else
	{
		shader->SetBool("hasNormal", false);
	}

    glBindVertexArray(_mVAO);
    glDrawArrays(GL_TRIANGLES, 0, _mVertCount);
    glBindVertexArray(0);
}
