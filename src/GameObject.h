#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Mesh;
class Shader;

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
	~GameObject();

	void Update(const float dt);
	void Render(GLuint programNum, Shader* shader);
	
	void AddMesh(Mesh* mesh) { _mMeshes.push_back(mesh); }

	Mesh* GetMesh(int index = 0) { return _mMeshes[index]; }
	unsigned int GetNumMeshes() { return _mMeshes.size(); }

	Transform GetTransform() { return _mTransform; }
	void SetTransform(Transform transform) { _mTransform = transform; }
	void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		_mTransform.position = position;
		_mTransform.rotation = rotation;
		_mTransform.scale = scale;
	}

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetPosition(glm::vec3 position) { 
		_mTransform.position = position;
		_mModelMatrix = glm::translate(_mModelMatrix, _mTransform.position);
	}

	glm::vec3 GetPosition() { return _mTransform.position; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetRotation(glm::vec3 rotation) { 
		_mTransform.rotation = rotation;
		_mModelMatrix = glm::rotate(_mModelMatrix, _mTransform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		_mModelMatrix = glm::rotate(_mModelMatrix, _mTransform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		_mModelMatrix = glm::rotate(_mModelMatrix, _mTransform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	glm::vec3 GetRotation() { return _mTransform.rotation; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetScale(glm::vec3 scale) { 
		_mTransform.scale = scale;
		_mModelMatrix = glm::scale(_mModelMatrix, _mTransform.scale);
	}
	glm::vec3 GetScale() { return _mTransform.scale; }

	GameObject* GetChild(std::string name) { 
		if (_mChildren.find(name) != _mChildren.end())
			return _mChildren[name];

		return nullptr;
	}

protected:
	// Transform
	Transform _mTransform;

	// Mesh
	std::vector<Mesh*> _mMeshes;

	// Model
	glm::mat4 _mModelMatrix;

	// Children
	std::unordered_map<std::string, GameObject*> _mChildren;
};

// GameObject needs to know which components are attached to each gameobject.

#endif // GAMEOBJECT_H
