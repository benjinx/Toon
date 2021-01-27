#ifndef TEMPORALITY_SCENE_H
#define TEMPORALITY_SCENE_H

#include <Temporality/Axis.hpp>
#include <Temporality/Config.hpp>
#include <Temporality/Scene/Entity.hpp>
#include <Temporality/Mesh.hpp>
#include <Temporality/Skybox.hpp>
//#include <Temporality/Shader.hpp>

#include <memory>
#include <unordered_map>

namespace Temporality {

class TEMPORALITY_ENGINE_API Scene : public Entity
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

} // namespace Temporality

#endif // TEMPORALITY_SCENE_H