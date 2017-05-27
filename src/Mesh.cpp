#include "Mesh.h"

#include "Camera.h"
#include "Shader.h"
#include "Material.h"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normal, std::vector<glm::vec2> texCoords)
{
    // VBO, VAO.
    glGenVertexArrays(1, &_mVAO);
    glBindVertexArray(_mVAO);

    GLuint vbos[3];
    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_POSITION);

    // CalcTBN(vertices, texCoords);

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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _mVertCount = vertices.size();
    _mRotSpeed  = 0.25f;
    _mSel       = 0.0f;
}

glm::mat4 Mesh::CalcTBN(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords)
{
    glm::vec3 E2m1  = vertices[1] - vertices[0];
    glm::vec2 uv2m1 = texCoords[1] - texCoords[0];

    glm::vec3 E3m1  = vertices[3] - vertices[0];
    glm::vec2 uv3m1 = texCoords[3] - texCoords[0];

    // T
    glm::vec3 T;
    if (uv2m1.x != 0)
        T = E2m1 / uv2m1.x;
    else // use Edge 3-1 instead.. think use the same uvs
        T = E3m1 / uv3m1.x;

    T = glm::normalize(T);

    // N
    glm::vec3 N = glm::cross(E2m1, E3m1);
    N           = glm::normalize(N);

    // B
    glm::vec3 B = glm::cross(T, N);

    // Matrix creation
    glm::mat4 TBN = glm::mat3(T, B, N);

    return TBN;
}

void Mesh::Render(GLuint programNum, Shader * shader)
{
    glUseProgram(shader->GetShaderProgramIDs()[programNum]);

    _mModelMat = glm::rotate(_mModelMat, glm::radians(_mRotSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "modelMat"), 1, false,
                       (GLfloat*)&_mModelMat);
    //glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "viewMat"), 1, false, (GLfloat*)&Camera::instance().GetViewMat());
    //glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "projMat"), 1, false, (GLfloat*)&Camera::instance().GetProjectionMat());

	glm::vec2 resolution = Camera::instance().GetResolution();
	glUniform2fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "resolution"), 1, (GLfloat*)&resolution);

    glm::mat4 mvp = Camera::instance().GetProjectionMat() * Camera::instance().GetViewMat() * _mModelMat;
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "mvp"), 1, false, (GLfloat*)&mvp);

    glm::vec4 lightPos = glm::vec4(0.0f, 5.0f, 5.0f, 1.0f);
    glUniform4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "lightPos"), 1, (GLfloat*)&lightPos);

    glm::vec3 camPos = Camera::instance().GetCameraPos();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    glUniform4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "eyePos"), 1, (GLfloat*)&eyePos);

	if (programNum == 0)
		glUniform1f(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "sel"), (GLfloat)_mSel);

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
