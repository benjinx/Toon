#include "Utils.hpp"

#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Material.hpp"

namespace Utils
{
	unsigned char* LoadPng(std::string filename, int& w, int& h, int& bpp)
	{
		filename = std::string(RESOURCES_DIR) + filename;

		// Remember to call stbi_image_free(image) after using the image and before another.
		// bpp - bits per pixel
		// 32 = RGBA = 4 * 8
		// 24 = RGB = 3 * 8
		//stbi_set_flip_vertically_on_load(true);

		unsigned char* image = stbi_load(filename.c_str(), &w, &h, &bpp, STBI_rgb_alpha);

		return image;
	}

	void FreePng(unsigned char* img) { stbi_image_free(img); }

	GLuint LoadTexture(std::string filename)
	{
		filename = std::string(RESOURCES_DIR) + filename;

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

	GameObject* LoadObj(std::string filename)
	{
		filename = std::string(RESOURCES_DIR) + filename;
		
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_FlipUVs);

		if (!scene)
		{
			std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
			return nullptr;
		}

		std::string dirname = GetDirname(filename) + "/";

		std::vector<Mesh*> meshes;

		// ProcessNode
		ProcessNode(scene->mRootNode, scene, meshes, dirname);

		GameObject* gobj = new GameObject();
		gobj->AddMesh(meshes[0]);

		return gobj;
	}

	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, meshes, dirname));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, meshes, dirname);
		}
	}

	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// Vertices
			glm::vec3 vertice(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertices.push_back(vertice);

			// Normals
			if (mesh->HasNormals())
			{
				glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				normals.push_back(normal);
			}

			// TexCoords
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoord(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				texCoords.push_back(texCoord);
			}
			else
				texCoords.push_back(glm::vec2(0.0f, 0.0f));

			// Tangents
			if (mesh->HasTangentsAndBitangents())
			{
				glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				tangents.push_back(tangent);

				glm::vec3 bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
				bitangents.push_back(bitangent);
			}
		}

		Mesh* newMesh = new Mesh(vertices, normals, texCoords, tangents, bitangents);
		Material* newMat = nullptr;

		// Materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// Ambient
			std::string ambientTex = GetMaterialTextureName(material, aiTextureType_AMBIENT, dirname);

			// Diffuse
			std::string diffuseTex = GetMaterialTextureName(material, aiTextureType_DIFFUSE, dirname);

			// Specular
			std::string specularTex = GetMaterialTextureName(material, aiTextureType_SPECULAR, dirname);

			// Normal
			std::string normalTex = GetMaterialTextureName(material, aiTextureType_NORMALS, dirname);

			aiColor4D aiAmb;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &aiAmb);
			float amb[3] = { aiAmb.r, aiAmb.g, aiAmb.b };

			aiColor4D aiDiff;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aiDiff);
			float diff[3] = { aiDiff.r, aiDiff.g, aiDiff.b };

			aiColor4D aiSpec;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &aiSpec);
			float spec[3] = { aiSpec.r, aiSpec.g, aiSpec.b };

			aiColor4D aiShininess;
			aiGetMaterialColor(material, AI_MATKEY_SHININESS, &aiShininess);
			glm::vec3 shininess(aiShininess.r, aiShininess.g, aiShininess.b);

			// Notes:
			// Remove dissolve from material it's pointless
			// Add all the other materials, Albedo, Metallic, roughness, AO?, etc


			newMat = new Material(amb, diff, spec, 0.0f, 0.0f, ambientTex, diffuseTex, specularTex, normalTex);
			newMesh->SetMaterial(newMat);
		}

		return newMesh;
	}

	std::string GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname)
	{
		std::string texName;
		for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			texName = dirname + str.C_Str();
			printf("Loading Material: %s\n", texName.c_str());
		}
		return texName;
	}

}
