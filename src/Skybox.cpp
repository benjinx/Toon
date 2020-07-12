#include <Skybox.hpp>

#include <App.hpp>
#include <OpenGL.hpp>
#include <Log.hpp>
#include <Utils.hpp>

#include "stb/stb_image.h"

Skybox::Skybox()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &_mVAO);
    glGenBuffers(1, &_mVBO);
    glBindVertexArray(_mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // Pass in position at loc 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    _mShader = App::Inst()->GetShader("skybox");
}

Skybox::~Skybox()
{

}

void Skybox::LoadCubemap(std::vector<std::string> faces)
{
    glGenTextures(1, &_mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _mTexture);

    // This will iterate through all cube map targets by doing + i, normally they are
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Front
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Back
    int width, height, nrChannels;
    unsigned char* data;
    int i = 0;

    const auto& paths = Utils::GetResourcePaths();
    for (auto& f : faces)
    {
        for (auto& p : paths)
        {
            std::string fullFilename = p + "/" + f;

            data = stbi_load(fullFilename.c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                LogLoad("Cubemap texture Loaded at path: %s", fullFilename.c_str());
                break;
            }
            else
            {
                LogWarn("Cubemap texture failed to load at path: %s", fullFilename.c_str());
                stbi_image_free(data);
            }
        }
        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::PreRender()
{
    // Skybox related here
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _mTexture);
}

void Skybox::Render()
{
    // Our render.
    App* app = App::Inst();

    //glDepthMask(GL_FALSE);
    //auto skybox = app->GetShader("skybox");
    glDepthFunc(GL_LEQUAL);
    _mShader->Use();
    glm::mat4 view = glm::mat3(app->GetCurrentCamera()->GetView());
    glm::mat4 proj = app->GetCurrentCamera()->GetProjection();
    _mShader->SetMat4("viewMat", view);
    _mShader->SetMat4("projMat", proj);

    glBindVertexArray(_mVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _mTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Cleanup
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
    //glDepthMask(GL_TRUE);

    glClear(GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LESS);

    // Render the rest
    //GameObject::Render();
}