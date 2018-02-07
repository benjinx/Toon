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
	_mRigidbody.UpdateFirstOrder(this, dt);

	const float res = 0.6f;
	const float min = 0.0f;
	_mRigidbody.ClampToGround(this, min, res);

	// Steve's Dusk
	//_transform = _baseTransform;
	//_transform = glm::translate(_transform, _position);
	//_transform = glm::rotate(_transform, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//_transform = glm::rotate(_transform, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//_transform = glm::rotate(_transform, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//_transform = glm::scale(_transform, _scale);

	// GLM

	//_mModelMatrix = glm::translate(_mModelMatrix, _mTransform.position);
	//_mModelMatrix = glm::rotate(_mModelMatrix, _mTransform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//_mModelMatrix = glm::rotate(_mModelMatrix, _mTransform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//_mModelMatrix = glm::rotate(_mModelMatrix, _mTransform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//_mModelMatrix = glm::scale(_mModelMatrix, _mTransform.scale);

	//// Matrix math from scratch
	
	//// Position
	///***	 ***\
	//|| 1 0 0 X ||
	//|| 0 1 0 Y ||
	//|| 0 0 1 Z ||
	//|| 0 0 0 1 ||
	//\***	 ***/

	//_mModelMatrix[3][0] = _mTransform.position.x;
	//_mModelMatrix[3][1] = _mTransform.position.y;
	//_mModelMatrix[3][2] = _mTransform.position.z;

	//// Scale
	///***	 ***\
	//|| X 0 0 0 ||
	//|| 0 Y 0 0 ||
	//|| 0 0 Z 0 ||
	//|| 0 0 0 1 ||
	//\***	 ***/

	//_mModelMatrix[0][0] = _mTransform.scale.x;
	//_mModelMatrix[1][1] = _mTransform.scale.y;
	//_mModelMatrix[2][2] = _mTransform.scale.z;

	//// Rotation
	//// https://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations
	//// https://open.gl/transformations

	//glm::mat4 XRotationMatrix, YRotationMatrix, ZRotationMatrix;

	//XRotationMatrix[1][1] = glm::cos(_mTransform.rotation.x);
	//XRotationMatrix[1][2] = -glm::sin(_mTransform.rotation.x);
	//XRotationMatrix[2][1] = glm::sin(_mTransform.rotation.x);
	//XRotationMatrix[2][2] = glm::cos(_mTransform.rotation.x);

	//_mModelMatrix = XRotationMatrix * _mModelMatrix;

	//YRotationMatrix[0][0] = glm::cos(_mTransform.rotation.y);
	//YRotationMatrix[0][2] = glm::sin(_mTransform.rotation.y);
	//YRotationMatrix[2][0] = -glm::sin(_mTransform.rotation.y);
	//YRotationMatrix[2][2] = glm::cos(_mTransform.rotation.y);

	//_mModelMatrix = YRotationMatrix * _mModelMatrix;

	//ZRotationMatrix[0][0] = glm::cos(_mTransform.rotation.z);
	//ZRotationMatrix[0][1] = -glm::sin(_mTransform.rotation.z);
	//ZRotationMatrix[1][0] = glm::sin(_mTransform.rotation.z);
	//ZRotationMatrix[1][1] = glm::cos(_mTransform.rotation.z);

	//_mModelMatrix = ZRotationMatrix * _mModelMatrix;
}

void GameObject::Render(GLuint programNum, Shader* shader)
{
	for (Mesh* mesh : _mMeshes)
	{
		mesh->Render(programNum, shader, GetModelMatrix());
	}
}