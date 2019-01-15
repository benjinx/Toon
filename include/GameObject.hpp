#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <vector>

class Shader;
class Model;

//
class GameObject
{
public:

	GameObject();
	GameObject(glm::vec3 position);
	GameObject(std::string filename);
	virtual ~GameObject();

	virtual void Update(const float dt);
	void Render();
	void InitAxis();
	void DrawAxis();

	void SetShader(Shader* shader);

	void SetParent(GameObject* parent) { _mParent = parent; }
	GameObject* GetParent() const { return _mParent; }
	void AddChild(GameObject* child) { _mChildren.push_back(child); }
	std::vector<GameObject*> GetChildren() { return _mChildren; }

	//// LOCAL
	// Local Transform
	void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	{
		_mPosition = position;
		_mRotation = rotation;
		_mScale = scale;
	}

	glm::mat4 GetTransform() const
	{ 
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, _mPosition);
		transform *= glm::mat4_cast(_mRotation);
		transform = glm::scale(transform, _mScale);
		return transform;
	}

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetPosition(glm::vec3 position) {
		_mPosition = position;
	}

	glm::vec3 GetPosition() const { return _mPosition; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetRotation(glm::quat rotation) {
		_mRotation = rotation;
	}

	glm::quat GetRotation() const { return _mRotation; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetScale(glm::vec3 scale) {
		_mScale = scale;
	}

	glm::vec3 GetScale() const { return _mScale; }
	//// END LOCAL

	//// WORLD
	// World Transform
	glm::mat4 GetWorldTransform() const
	{
		if (GetParent())
		{
			return GetParent()->GetTransform() * GetTransform();
		}

		return GetTransform();
	}

	glm::vec3 GetWorldPosition() const
	{
		if (GetParent())
		{
			return GetParent()->GetPosition() + GetPosition();
		}

		return GetPosition();
	}

	glm::quat GetWorldRotation() const
	{
		if (GetParent())
		{
			return GetParent()->GetRotation() * GetRotation();
		}

		return GetRotation();
	}

	glm::vec3 GetWorldScale() const
	{
		if (GetParent())
		{
			return GetParent()->GetScale() * GetScale();
		}

		return GetScale();
	}
	//// END WORLD
	
protected:
	// Pos, rot, scale
	glm::vec3 _mPosition = glm::vec3(0.0f),
			  _mScale = glm::vec3(1.0f);
	glm::quat _mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	// Axis Rendering
	GLuint _mVAO;
	GLuint _mVBO[2];

	// Gobjs Shader
	Shader* _mShader;

	// Model
	Model* _mModel = nullptr;

	// Parent
	GameObject* _mParent = nullptr;

	// Children
	std::vector<GameObject*> _mChildren;
};
#endif // GAMEOBJECT_HPP
