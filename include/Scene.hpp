#ifndef SCENE_H
#define SCENE_H

#include <Axis.hpp>
#include <Config.hpp>
#include <GameObject.hpp>
#include <Mesh.hpp>

#include <memory>
#include <unordered_map>

class Scene : public GameObject
{
public:

    Scene() = default;
    virtual ~Scene();
    
    virtual void Start();
    virtual void Pause() { }
    virtual void Resume() { }

    virtual void Update(float dt);
    virtual void Render();

    bool Load(std::string filename);

    GameObject* GetGameObject(std::string name);

    // Ui Options.
    static void Options();

    void AddGameObject(std::string name, GameObject* gameObject);
    GameObject* AddGameObject();
    GameObject* AddGameObject(std::string name);
    void AddGameObject(GameObject* gobj);

private:
    static bool _sShowAxis;

    Axis* _mSceneAxis;

    glm::mat4 _mSceneTransform = glm::mat4(1);
};

#endif // SCENE_H