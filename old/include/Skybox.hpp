#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GameObject.hpp>
#include <Shader.hpp>

class Skybox : public GameObject
{
public:
    Skybox();
    ~Skybox();

    void LoadCubemap(std::vector<std::string> faces);

    void PreRender();
    void Render() override;

    void SetShader(Shader* shader) { _mShader = shader; }

private:
    unsigned int _mVAO, _mVBO, _mTexture;

    Shader* _mShader = nullptr;
};

#endif // SKYBOX_HPP