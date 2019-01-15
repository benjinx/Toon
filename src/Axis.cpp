#include "Axis.hpp"

#include <App.hpp>
#include <Model.hpp>
#include <Shader.hpp>

Axis::Axis()
{
	if (!_mMesh)
	{
		GLuint vao;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint indices[] = {
			0, 1,
			2, 3,
			4, 5,
		};

		GLfloat position[] = {
			0.0f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f,
		};

		GLfloat color[] = {
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
		};

		GLuint vbos[3];
		glGenBuffers(3, vbos);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
		glVertexAttribPointer(AttributeID::POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AttributeID::POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
		glVertexAttribPointer(AttributeID::COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AttributeID::COLOR);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(3, vbos);

		GLsizei count = sizeof(indices) / sizeof(indices[0]);

		_mMesh = std::make_shared<Mesh>(
			Mesh(vao,
				(GLenum)GL_LINES,
				(GLsizei)count,
				(GLenum)GL_UNSIGNED_INT,
				(GLsizei)0,
				nullptr));
	}
}

void Axis::Render(glm::mat4 transform)
{
	Shader* shader = App::Inst()->GetShader("axis");
	glClear(GL_DEPTH_BUFFER_BIT);
	_mMesh->Render(shader, transform);
}