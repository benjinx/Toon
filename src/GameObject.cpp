#include "GameObject.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <Shader.hpp>

GameObject::GameObject()
{
	InitAxis();
}

GameObject::GameObject(glm::vec3 position)
{
	InitAxis();
}

GameObject::GameObject(std::string filename)
{
	_mModel = new Model(filename);
	InitAxis();
}

GameObject::~GameObject()
{

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

void GameObject::InitAxis()
{
	glGenVertexArrays(1, &_mVAO);
	glBindVertexArray(_mVAO);
	GLfloat arr[] = { 0, 0, 0,
					  0.5f, 0, 0,
					  0, 0, 0,
				   	  0, 0.5f, 0,
					  0, 0, 0,
					  0, 0, 0.5f };

	GLfloat color[] = { 1.0f, 0, 0,
						1.0f, 0, 0,
						0, 1.0f, 0,
						0, 1.0f, 0,
						0, 0, 1.0f,
						0, 0, 1.0f};

	glGenBuffers(2, _mVBO);

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, _mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// Color
	glBindBuffer(GL_ARRAY_BUFFER, _mVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameObject::DrawAxis()
{
	Shader* shader = App::Inst()->GetShader("axis");

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader->GetShaderID());

	const auto& view = App::Inst()->GetCurrentCamera()->GetView();
	const auto& proj = App::Inst()->GetCurrentCamera()->GetProjection();
	const auto& model = GetTransform();	

	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "modelMat"), 1, false, (GLfloat*)&model);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "viewMat"), 1, false, (GLfloat*)&view);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "projMat"), 1, false, (GLfloat*)&proj);

	glBindVertexArray(_mVAO);
	glDrawArrays(GL_LINES, 0, 18);

	glBindVertexArray(0);
}