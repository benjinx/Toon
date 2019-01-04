#include "Utils.hpp"

#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Material.hpp"
#include <sstream>
#include "Log.hpp"

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

	Mesh* Get2DMesh(glm::vec4 screenCords, glm::vec4 textureCords)
	{
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
	}
}
