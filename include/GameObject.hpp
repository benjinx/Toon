#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Axis.hpp>
#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Material.hpp>

#include <vector>
#include <memory>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiString;

//
class GameObject
{
public:

    GameObject();
    GameObject(glm::vec3 position);
    virtual ~GameObject();

    virtual void Update(const float dt);
    void Render();

    void SetShader(Shader * shader) {
        _mShader = std::move(shader);
    }

    void SetParent(GameObject* parent) { 
        _mParent = parent;
    }

    GameObject* GetParent() const { 
        return _mParent;
    }

    GameObject* FindGameObject(std::string name);

    void AddChild(std::unique_ptr<GameObject>&& child);

    void RenderAxis();

    void SetName(std::string name) { _mName = name; }
    std::string GetName() { return _mName; }

    void SetModel(std::unique_ptr<Model> model) { _mModel = std::move(model); }

    bool Load(std::string filename);

    // Local Transform
    void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

    glm::mat4 GetTransform() const;
    glm::vec3 GetPosition() const { return _mPosition; }
    glm::quat GetRotation() const { return _mRotation; }
    glm::vec3 GetScale() const { return _mScale; }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetPosition(glm::vec3 position) {
        _mPosition = position;
    }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetRotation(glm::quat rotation) {
        _mRotation = rotation;
    }

    // Remember matrix order is Translate (Position), Rotate, Scale
    void SetScale(glm::vec3 scale) {
        _mScale = scale;
    }

    // World Transforms
    glm::mat4 GetWorldTransform() const;
    glm::vec3 GetWorldPosition() const;
    glm::quat GetWorldRotation() const;
    glm::vec3 GetWorldScale() const;

protected:

    // Children
    std::vector<std::unique_ptr<GameObject>> _mChildren;
    
private:
    // Pos, rot, scale
    glm::vec3 _mPosition = glm::vec3(0.0f),
              _mScale = glm::vec3(1.0f);
    glm::quat _mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Axis of the object
    Axis* _mSceneAxis = nullptr;

    // Gobjs Shader
    Shader * _mShader;

    // Model
    std::unique_ptr<Model> _mModel = nullptr;

    // Parent
    GameObject* _mParent = nullptr;

    // Object name
    std::string _mName;

    // Load Materials
    std::unique_ptr<Material> processMaterial(const aiScene * scene, std::string dir, aiMaterial* material);

    // Part of loading function
    std::unique_ptr<GameObject> processNode(const aiScene * scene, std::string dir, aiNode* node, std::unordered_map<std::string, std::unique_ptr<GameObject>>& mapOfLights, std::unordered_map<std::string, std::unique_ptr<GameObject>>& mapOfCameras);

    // Load Mesh
    std::unique_ptr<Mesh> processMesh(const aiScene * scene, std::string dir, aiMesh* mesh);

    // process textures
    std::unique_ptr<Texture> processTexture(const aiScene * scene, std::string dir, const aiString& filename);

};
#endif // GAMEOBJECT_HPP
