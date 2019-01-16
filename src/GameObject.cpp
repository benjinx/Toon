#include "GameObject.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <Shader.hpp>

GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 position)
{
}

GameObject::GameObject(std::string filename)
{
	_mModel = new Model(filename);
}

GameObject::~GameObject()
{
	for (auto c : _mChildren)
		delete c;

	delete _mModel;
}

void GameObject::Update(const float dt)
{
}

void GameObject::Render()
{
	if (_mShader != nullptr && _mModel != nullptr)
	{
		_mModel->Render(_mShader, GetWorldTransform());
	}
}

void GameObject::SetShader(Shader* shader)
{
	_mShader = shader;
}