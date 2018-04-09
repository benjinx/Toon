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

    glBindVertexArray(0);
}
