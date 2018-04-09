#ifndef UTILITIES_H
#define UTILITIES_H

#include "Common.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class GameObject;
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
class Mesh;

enum LogLevel
{
	LOG_INFO = 0,
	LOG_WARNING,
	LOG_ERROR,
};

namespace Utils
{
	unsigned char* LoadPng(std::string filename, int& w, int& h, int& bpp);

	void FreePng(unsigned char* img);

	GLuint LoadTexture(std::string filename);

	std::string GetBasename(const std::string& path);

	std::string GetDirname(const std::string& path);

	GameObject* LoadObj(std::string filename);
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname);
	std::string GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname);

	void Log(LogLevel flag, std::string infoStr);
}

#endif // UTILITIES_H
