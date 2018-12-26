#include "Utils.hpp"

#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Material.hpp"
#include <sstream>

namespace Utils
{
	unsigned char* LoadPng(std::string filename, int& w, int& h, int& bpp)
	{
        const auto& paths = GetResourcePaths();
        FILE * file = NULL;

	    printf("Loading: [%s]\n", filename.c_str());

        for (const std::string& p : paths) {
            std::string fullFilename = p + "/" + filename;
            file = fopen(fullFilename.c_str(), "rb");

            if (!file) continue;

		    printf("Loaded:  [%s]\n", fullFilename.c_str());

            // Remember to call stbi_image_free(image) after using the image and before another.
    		// bpp - bits per pixel
    		// 32 = RGBA = 4 * 8
    		// 24 = RGB = 3 * 8
    		//stbi_set_flip_vertically_on_load(true);

    		unsigned char* image = stbi_load_from_file(file, &w, &h, &bpp, STBI_rgb_alpha);
            fclose(file);
            return image;
        }

        return nullptr;
	}

	void FreePng(unsigned char* img) { stbi_image_free(img); }

	GLuint LoadTexture(std::string filename)
	{
		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// texture wrappin para
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		// texture filtering para
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load img
		int            width, height, bpp;
		unsigned char* img = LoadPng(filename, width, height, bpp);

		if (!img) {
			fprintf(stderr, "Failed to load [%s]", filename.c_str());
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);

		FreePng(img);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}

	std::string GetBasename(const std::string& path)
	{
		size_t pos = path.find_last_of("/\\");
		if (std::string::npos != pos)
		{
			return path.substr(pos);
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
