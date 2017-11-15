#include "Utils.h"

#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "tiny_obj_loader/tiny_obj_loader.h"
#include "assimp/config.h"

namespace Utils
{
	unsigned char* LoadPng(std::string filename, int& w, int& h, int& bpp)
	{
		// Remember to call stbi_image_free(image) after using the image and before another.
		// bpp - bits per pixel
		// 32 = RGBA = 4 * 8
		// 24 = RGB = 3 * 8
		stbi_set_flip_vertically_on_load(true);

		unsigned char* image = stbi_load(filename.c_str(), &w, &h, &bpp, STBI_rgb_alpha);

		return image;
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

	std::vector<Model*> LoadObjs(std::string filename)
	{
		//
		bool                             ret;
		std::string                      err;
		tinyobj::attrib_t                attrib;
		std::vector<tinyobj::shape_t>    shapes;
		std::vector<tinyobj::material_t> materials;

		std::string dirname = GetDirname(filename) + "/";

		ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), dirname.c_str());

		if (!ret)
		{
			fprintf(stderr, "Failed to load %s: %s\n", filename.c_str(), err.c_str());
			std::vector<Model*> errModel = {};
			return errModel;
		}

		bool hasNorms     = !attrib.normals.empty();
		bool hasTexcoords = !attrib.texcoords.empty();

		std::vector<Model*> models;
		std::vector<Mesh*>  meshes;

		for (tinyobj::shape_t& shape : shapes)
		{
			printf("Shape: %s\n", shape.name.c_str());
			tinyobj::mesh_t&     mesh = shape.mesh;
			tinyobj::material_t* mat  = NULL;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> texCoords;

			vertices.resize(mesh.indices.size());

			if (hasNorms)
				normals.resize(mesh.indices.size());

			if (hasTexcoords)
				texCoords.resize(mesh.indices.size());

			size_t index = 0;
			for (size_t f = 0; f < mesh.num_face_vertices.size(); ++f)
			{
				size_t fv = mesh.num_face_vertices[f];
				for (size_t v = 0; v < fv; ++v)
				{
					tinyobj::index_t idx = mesh.indices[index + v];

					vertices[(index + v)] =
						glm::vec3(attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1],
								  attrib.vertices[3 * idx.vertex_index + 2]);

					if (hasNorms)
					{
						normals[(index + v)] =
							glm::vec3(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1],
									  attrib.normals[3 * idx.normal_index + 2]);
					}

					if (hasTexcoords)
					{
						texCoords[(index + v)] = glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0],
														   attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index += fv;
			}

			Mesh*     newMesh = new Mesh(vertices, normals, texCoords);
			Material* newMat  = nullptr;

			if (!mesh.material_ids.empty() && mesh.material_ids[0] >= 0)
			{
				mat = &materials[mesh.material_ids[0]];
			}

			if (mat != nullptr)
			{
				printf("Material: %s\n", mat->name.c_str());

				std::string ambient_tex = (mat->ambient_texname.empty() ? std::string() : dirname + mat->ambient_texname);
				std::string diffuse_tex = (mat->diffuse_texname.empty() ? std::string() : dirname + mat->diffuse_texname);
				std::string specular_tex = (mat->specular_texname.empty() ? std::string() : dirname + mat->specular_texname);
				std::string bump_tex = (mat->bump_texname.empty() ? std::string() : dirname + mat->bump_texname);

				newMat = new Material(mat->ambient, mat->diffuse, mat->specular, mat->dissolve, mat->shininess,
									  ambient_tex, diffuse_tex, specular_tex, bump_tex);
				newMesh->SetMaterial(newMat);
			}

			Model* newModel = new Model();
			newModel->AddMesh(newMesh);
			models.push_back(newModel);
		}
		return models;
	}

	Model* LoadObj(std::string filename)
	{
		//
		bool                             ret;
		std::string                      err;
		tinyobj::attrib_t                attrib;
		std::vector<tinyobj::shape_t>    shapes;
		std::vector<tinyobj::material_t> materials;

		std::string dirname = GetDirname(filename) + "/";

		ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), dirname.c_str());

		if (!ret)
		{
			fprintf(stderr, "Failed to load %s: %s\n", filename.c_str(), err.c_str());
			Model* errModel = {};
			return errModel;
		}

		bool hasNorms = !attrib.normals.empty();
		bool hasTexcoords = !attrib.texcoords.empty();

		Model* newModel = new Model();
		std::vector<Mesh*>  meshes;

		for (tinyobj::shape_t& shape : shapes)
		{
			printf("Shape: %s\n", shape.name.c_str());
			tinyobj::mesh_t&     mesh = shape.mesh;
			tinyobj::material_t* mat = NULL;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> texCoords;

			vertices.resize(mesh.indices.size());

			if (hasNorms)
				normals.resize(mesh.indices.size());

			if (hasTexcoords)
				texCoords.resize(mesh.indices.size());

			size_t index = 0;
			for (size_t f = 0; f < mesh.num_face_vertices.size(); ++f)
			{
				size_t fv = mesh.num_face_vertices[f];
				for (size_t v = 0; v < fv; ++v)
				{
					tinyobj::index_t idx = mesh.indices[index + v];

					vertices[(index + v)] =
						glm::vec3(attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1],
							attrib.vertices[3 * idx.vertex_index + 2]);

					if (hasNorms)
					{
						normals[(index + v)] =
							glm::vec3(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1],
								attrib.normals[3 * idx.normal_index + 2]);
					}

					if (hasTexcoords)
					{
						texCoords[(index + v)] = glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0],
							attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index += fv;
			}

			Mesh*     newMesh = new Mesh(vertices, normals, texCoords);
			Material* newMat = nullptr;

			if (!mesh.material_ids.empty() && mesh.material_ids[0] >= 0)
			{
				mat = &materials[mesh.material_ids[0]];
			}

			if (mat != nullptr)
			{
				printf("Material: %s\n", mat->name.c_str());

				std::string ambient_tex = (mat->ambient_texname.empty() ? std::string() : dirname + mat->ambient_texname);
				std::string diffuse_tex = (mat->diffuse_texname.empty() ? std::string() : dirname + mat->diffuse_texname);
				std::string specular_tex = (mat->specular_texname.empty() ? std::string() : dirname + mat->specular_texname);
				std::string bump_tex = (mat->bump_texname.empty() ? std::string() : dirname + mat->bump_texname);

				newMat = new Material(mat->ambient, mat->diffuse, mat->specular, mat->dissolve, mat->shininess,
					ambient_tex, diffuse_tex, specular_tex, bump_tex);
				newMesh->SetMaterial(newMat);
			}

			newModel->AddMesh(newMesh);
		}
		return newModel;
	}
	
	//Model* LoadObjN(std::string filename)
	//{

	//}
}
