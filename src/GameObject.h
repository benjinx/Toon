#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Need to make sure to use transform data for modelMatrix.
struct Transform
{
	glm::vec3 position, rotation, scale;
};

//
class GameObject
{
public:
	GameObject() {};
	GameObject(glm::vec3 position);

	void Update(const float dt);

	Transform GetTransform() { return _mTransform; }
	void SetTransform(Transform transform) { _mTransform = transform; }
	void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		_mTransform.position = position;
		_mTransform.rotation = rotation;
		_mTransform.scale = scale;
	}
	void SetPosition(glm::vec3 position) { _mTransform.position = position; }
	glm::vec3 GetPosition() { return _mTransform.position; }

protected:
	// Transform
	Transform _mTransform;

	// Model
	glm::mat4 _mModelMatrix;
};

// GameObject needs to know which components are attached to each gameobject.

#endif // GAMEOBJECT_H