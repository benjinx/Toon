#include "Mesh.h"

#include "Camera.h"
#include "Shader.h"
#include "Material.h"

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

//glm::mat4 Mesh::CalcTBN(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords)
//{
//	for (int i = 0; i < vertices.size(); i += 3)
//	{
//		glm::vec3 &v0 = vertices[i + 0];
//		glm::vec3 &v1 = vertices[i + 1];
//		glm::vec3 &v2 = vertices[i + 2];
//
//		glm::vec2 &uv0 = texCoords[i + 0];
//		glm::vec2 &uv1 = texCoords[i + 1];
//		glm::vec2 &uv2 = texCoords[i + 2];
//
//		glm::vec3 deltaPos1 = v1 - v0;
//		glm::vec3 deltaPos2 = v2 - v0;
//
//		glm::vec2 deltaUV1 = uv1 - uv0;
//		glm::vec2 deltaUV2 = uv2 - uv0;
//
//		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
//		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
//		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
//
//		tangents.push_back(tangent);
//		tangents.push_back(tangent);
//		tangents.push_back(tangent);
//
//		bitangents.push_back(bitangent);
//		bitangents.push_back(bitangent);
//		bitangents.push_back(bitangent);
//	}
//
//    //glm::vec3 E2m1  = vertices[1] - vertices[0];
//    //glm::vec2 uv2m1 = texCoords[1] - texCoords[0];
//
//    //glm::vec3 E3m1  = vertices[3] - vertices[0];
//    //glm::vec2 uv3m1 = texCoords[3] - texCoords[0];
//
//    //// T
//    //glm::vec3 T;
//    //if (uv2m1.x != 0)
//    //    T = E2m1 / uv2m1.x;
//    //else // use Edge 3-1 instead.. think use the same uvs
//    //    T = E3m1 / uv3m1.x;
//
//    //T = glm::normalize(T);
//
//    //// N
//    //glm::vec3 N = glm::cross(E2m1, E3m1);
//    //N           = glm::normalize(N);
//
//    //// B
//    //glm::vec3 B = glm::cross(T, N);
//
//    //// Matrix creation
//    //glm::mat4 TBN = glm::mat3(T, B, N);
//
//    //return TBN;
//	return glm::mat4();
//}

//void Mesh::Draw(GLuint programNum, Shader* shader, glm::mat4 modelMat)
//{
//	unsigned int diffuseN = 0;
//	unsigned int specularN = 0;
//
//	for (unsigned int i = 0; i < _mTextures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//		std::string num;
//		std::string name = _mTextures[i].type;
//		if (name == "texture_diffuse")
//			num = std::to_string(diffuseN++);
//		else if (name == "texture_specular")
//			num = std::to_string(specularN++);
//
//		glUniform1i(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], (name + num).c_str()), i);
//		glBindTexture(GL_TEXTURE_2D, _mTextures[i].id);
//	}
//	glActiveTexture(GL_TEXTURE0);
//
//	glBindVertexArray(_mVAO);
//	glDrawElements(GL_TRIANGLES, _mIndices.size(), GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//}

void Mesh::Render(GLuint programNum, Shader * shader, glm::mat4 modelMat)
{
    glUseProgram(shader->GetShaderProgramIDs()[programNum]);

    const auto& view = Camera::instance().GetViewMat();
    const auto& proj = Camera::instance().GetProjectionMat();

    glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "modelMat"), 1, false,
                       (GLfloat*)&modelMat);
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "viewMat"), 1, false, (GLfloat*)&view);
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "projMat"), 1, false, (GLfloat*)&proj);

    glm::mat4 mvp = Camera::instance().GetProjectionMat() * Camera::instance().GetViewMat() * modelMat;
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "mvp"), 1, false, (GLfloat*)&mvp);

	glm::vec4 lightPos = glm::vec4(5.0f, 2.0f, 2.0f, 1.0f);
    glUniform4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "lightPos"), 1, (GLfloat*)&lightPos);

    glm::vec3 camPos = Camera::instance().GetCameraPos();
	glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "eyePos"), 1, (GLfloat*)&eyePos);

	// Material values
	auto ambient = _mMaterial->GetAmbient();
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "ambient"), 1, (GLfloat*)&ambient);
	auto diffuse = _mMaterial->GetDiffuse();
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "diffuse"), 1, (GLfloat*)&diffuse);
	auto specular = _mMaterial->GetSpecular();
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "specular"), 1, (GLfloat*)&specular);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "shininess"), _mMaterial->GetShininess());

	_mMaterial->Bind();

    if (_mMaterial->AmbientTexExists())
    {
        glUniform1i(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "ambientTex"), TextureID::AMBIENT);
    }

    if (_mMaterial->DiffuseTexExists())
    {
        glUniform1i(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "diffuseTex"), TextureID::DIFFUSE);
    }

    if (_mMaterial->SpecularTexExists())
    {
        glUniform1i(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "specularTex"), TextureID::SPECULAR);
    }

    if (_mMaterial->BumpTexExists())
    {
        glUniform1i(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "bumpTex"), TextureID::BUMP);
    }

    glBindVertexArray(_mVAO);
    glDrawArrays(GL_TRIANGLES, 0, _mVertCount);
	//glDrawElements(GL_TRIANGLES, _mIndices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
