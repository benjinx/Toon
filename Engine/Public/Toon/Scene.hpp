#ifndef TOON_SCENE_H
#define TOON_SCENE_H

#include <Toon/Axis.hpp>
#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>
#include <Toon/Mesh.hpp>
#include <Toon/Skybox.hpp>
//#include <Toon/Shader.hpp>

#include <memory>
#include <unordered_map>

namespace Toon {

class TOON_ENGINE_API Scene : public Entity
{
public:

    Scene() = default;
    virtual ~Scene() = default;
    
    virtual void Start() {}
    virtual void Pause() {}
    virtual void Resume() {}

    virtual void Update(UpdateContext * ctx);
    virtual void Render(RenderContext * ctx);

    bool LoadScene(std::string filename);

    // Ui Options.
    static void Options();

    void CreateSkybox(std::vector<std::string> faces);
    Skybox* GetSkybox() { return _mSkybox.get(); }

private:
    glm::mat4 _mSceneTransform = glm::mat4(1);

    std::unique_ptr<Skybox> _mSkybox;
};

} // namespace Toon

#endif // TOON_SCENE_H