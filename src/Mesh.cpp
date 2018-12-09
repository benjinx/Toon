#include "Mesh.hpp"

#include "Camera.hpp"
#include "Shader.hpp"
#include "Material.hpp"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normal, std::vector<glm::vec2> texCoords, std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents)
{
    // VBO, VAO.
    glGenVertexArrays(1, &_mVAO);
    glBindVertexArray(_mVAO);

    glGenBuffers(5, _mVBOs);

    glBindBuffer(GL_ARRAY_BUFFER, _mVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(Attrib::POSITION);

    if (!normal.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, _mVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normal.size() * 3, normal.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(Attrib::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(Attrib::NORMAL);
    }

    if (!texCoords.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, _mVBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(Attrib::TEXCOORD);
    }

	if (!tangents.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, _mVBOs[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size() * 3, tangents.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(Attrib::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(Attrib::TANGENT);
	}

	if (!bitangents.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, _mVBOs[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bitangents.size() * 3, bitangents.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(Attrib::BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(Attrib::BITANGENT);
	}

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _mVertCount = vertices.size();
}

void Mesh::Render(Shader * shader, glm::mat4 modelMat)
{
	shader->Use();

    const auto& view = Camera::Inst().GetViewMat();
    const auto& proj = Camera::Inst().GetProjectionMat();

	shader->SetMat4("modelMat", modelMat);
	shader->SetMat4("viewMat", view);
	shader->SetMat4("projMat", proj);

	glm::mat4 mvp = proj * view * modelMat;
	shader->SetMat4("mvp", mvp);

    glm::vec3 camPos = Camera::Inst().GetCameraPos();
	glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
	shader->SetVec4("eyePos", eyePos);

	// Material values
	auto ambient = _mMaterial->GetAmbient();
	shader->SetVec3("material.ambientVal", ambient);

	auto diffuse = _mMaterial->GetDiffuse();
	shader->SetVec3("material.diffuseVal", diffuse);

	auto specular = _mMaterial->GetSpecular();
	shader->SetVec3("material.specularVal", specular);

	auto shininess = _mMaterial->GetShininess();
	shader->SetFloat("material.shininess", shininess);

	_mMaterial->Bind();

	//if (_mMaterial->AmbientTexExists())
	//{
	//	//shader->SetInt("ambientTex", Material::TexID::AMBIENT);
	//	shader->SetInt("material.ambientTex", Material::TexID::AMBIENT);
	//	shader->SetBool("hasAmbient", true);
	//}
	//else
	//{
	//	shader->SetBool("hasAmbient", false);
	//}

    if (_mMaterial->DiffuseTexExists())
    {
		//shader->SetInt("diffuseTex", Material::TexID::DIFFUSE);
		shader->SetInt("material.diffuse", Material::TexID::DIFFUSE);
		shader->SetBool("hasDiffuse", true);
    }
	else
	{
		shader->SetBool("hasDiffuse", false);
	}

    if (_mMaterial->SpecularTexExists())
    {
		//shader->SetInt("specularTex", Material::TexID::SPECULAR);
		shader->SetInt("material.specular", Material::TexID::SPECULAR);
		shader->SetBool("hasSpecular", true);
	}
	else
	{
		shader->SetBool("hasSpecular", false);
	}

    if (_mMaterial->NormalTexExists())
    {
		//shader->SetInt("normalTex", Material::TexID::NORMAL);
		shader->SetInt("material.normal", Material::TexID::NORMAL);
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
