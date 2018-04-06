#include "GameObject.h"

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject()
{
	// Physics
	_mRigidbody.SetMass(1.0f);
	_mRigidbody.SetFixedAcceleration(glm::vec3(0.0f, -9.81f, 0.0f));
	_mRigidbody.SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	_mRigidbody.SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));

	axisEnabled = false;
}

GameObject::GameObject(glm::vec3 position)
{
	_mTransform.position = position;

	// Physics
	_mRigidbody.SetMass(1.0f);
	_mRigidbody.SetFixedAcceleration(glm::vec3(0.0f, -9.81f, 0.0f));
	_mRigidbody.SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	_mRigidbody.SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));

	axisEnabled = false;
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

void GameObject::InitAxis()
{
	axisEnabled = true;
	glGenVertexArrays(1, &_mVAO);
	glBindVertexArray(_mVAO);
	GLfloat arr[] = { 0, 0, 0,
					  0.5f, 0, 0,
					  0, 0, 0,
				   	  0, 0.5f, 0,
					  0, 0, 0,
					  0, 0, 0 + 0.5f };

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

void GameObject::DrawAxis(GLuint programNum, Shader * shader)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader->GetShaderProgramIDs()[programNum]);

    const auto& view = Camera::instance().GetViewMat();
    const auto& proj = Camera::instance().GetProjectionMat();

	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "modelMat"), 1, false, (GLfloat*)&_mModelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "viewMat"), 1, false, (GLfloat*)&view);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderProgramIDs()[programNum], "projMat"), 1, false, (GLfloat*)&proj);

	glBindVertexArray(_mVAO);
	glDrawArrays(GL_LINES, 0, 18);

	glBindVertexArray(0);
}
