#include "Utils.hpp"

#include <Log.hpp>
#include <Mesh.hpp>
#include <Model.hpp>

#include <sstream>

namespace Utils
{
	std::string GetBasename(const std::string& path)
	{
		size_t pos = path.find_last_of("/\\");
		if (std::string::npos != pos)
		{
			return path.substr(pos + 1);
		}
		return path;
	}

	std::string GetDirname(const std::string& path)
	{
		size_t pos = path.find_last_of("/\\");
		if (std::string::npos != pos)
		{
			return path.substr(0, pos);
		}
		return std::string();
	}

	std::string GetExtension(const std::string& path)
	{
		size_t pivot = path.find_last_of('.');
		return (pivot == std::string::npos
			? std::string()
			: path.substr(pivot + 1));
	}

	Mesh* Get2DMesh(glm::vec4 screenCords, glm::vec4 textureCords)
	{
		GLuint vbos[2];
		GLuint vao;
		std::vector<glm::vec3> vertices = {
			{ screenCords[2], screenCords[1], 0 },
		{ screenCords[2], screenCords[3], 0 },
		{ screenCords[0], screenCords[3], 0 },
		{ screenCords[2], screenCords[1], 0 },
		{ screenCords[0], screenCords[3], 0 },
		{ screenCords[0], screenCords[1], 0 } };

		std::vector<glm::vec2> texCoords = {
			{ textureCords[2], textureCords[3] },
		{ textureCords[2], textureCords[1] },
		{ textureCords[0], textureCords[1] },
		{ textureCords[2], textureCords[3] },
		{ textureCords[0], textureCords[1] },
		{ textureCords[0], textureCords[3] } };

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(AttributeID::POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AttributeID::POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, texCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(AttributeID::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AttributeID::TEXCOORD);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		Mesh* m = new Mesh(vao, 0, 0, 0, 0, {});

		return m;
		// std::vector<glm::vec3> Vertices, {}, std::vector<glm::vec2> texCoords, {}, {}
		/*
		return new Mesh(
		{
		{screenCords[2], screenCords[1], 0},
		{screenCords[2], screenCords[3], 0},
		{screenCords[0], screenCords[3], 0},
		{screenCords[2], screenCords[1], 0},
		{screenCords[0], screenCords[3], 0},
		{screenCords[0], screenCords[1], 0} },
		{},
		{
		{textureCords[2], textureCords[3]},
		{textureCords[2], textureCords[1]},
		{textureCords[0], textureCords[1]},
		{textureCords[2], textureCords[3]},
		{textureCords[0], textureCords[1]},
		{textureCords[0], textureCords[3]} },
		{}, {});
		*/
	}
}
