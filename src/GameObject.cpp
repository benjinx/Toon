#include "GameObject.h"

#include "Mesh.h"
#include "Shader.h"

GameObject::GameObject()
{
	// Physics
	_mRigidbody.SetMass(1.0f);
	_mRigidbody.SetFixedAcceleration(glm::vec3(0.0f, -9.81f, 0.0f));
	_mRigidbody.SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	_mRigidbody.SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
}

GameObject::GameObject(glm::vec3 position)
{
	_mTransform.position = position;

	// Physics
	_mRigidbody.SetMass(1.0f);
	_mRigidbody.SetFixedAcceleration(glm::vec3(0.0f, -9.81f, 0.0f));
	_mRigidbody.SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	_mRigidbody.SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
}

GameObject::~GameObject()
{
	for (Mesh* mesh : _mMeshes)
	{
		delete mesh;
	}
}

void GameObject::Update(const float dt)
{
	//_mRigidbody.UpdateFirstOrder(this, dt);

	const float res = 0.6f;
	const float min = 0.0f;
	//_mRigidbody.ClampToGround(this, min, res);
}

void GameObject::Render(GLuint programNum, Shader* shader)
{
	for (Mesh* mesh : _mMeshes)
	{
		mesh->Render(programNum, shader, GetModelMatrix());
	}
}