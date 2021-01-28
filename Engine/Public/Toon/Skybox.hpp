#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <Toon/Entity.hpp>
//#include <Toon/Shader.hpp>

namespace Toon {

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

} // namespace Toon

#endif // SKYBOX_HPP