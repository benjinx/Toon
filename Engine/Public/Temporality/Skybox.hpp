#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <Temporality/Scene/Entity.hpp>
//#include <Temporality/Shader.hpp>

namespace Temporality {

class Skybox : public Entity
{
public:
    /*Skybox();
    ~Skybox();

    void LoadCubemap(std::vector<std::string> faces);

    void PreRender();
    void Render() override;

    void SetShader(Shader* shader) { _mShader = shader; }

private:
    unsigned int _mVAO, _mVBO, _mTexture;

    Shader* _mShader = nullptr;*/
};

} // namespace Temporality

#endif // SKYBOX_HPP