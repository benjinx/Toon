#include "GameObject.hpp"

#include "App.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

GameObject::GameObject()
{
	// Create Our object

	// Transform Defaults
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	InitAxis();
}

GameObject::GameObject(glm::vec3 position)
{
	// Transform Defaults
	SetPosition(position);
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	InitAxis();
}

GameObject::GameObject(std::string filename)
{
	LoadObj(filename);

	// Transform Defaults
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	InitAxis();
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
}

void GameObject::Render()
{
	if (_mShader != nullptr)
	{
		for (Mesh* mesh : _mMeshes)
		{
			mesh->Render(_mShader, GetModelMatrix());
		}
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

void GameObject::DrawAxis()
{
	Shader* shader = App::Inst()->GetShader("axis");

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader->GetShaderID());

    const auto& view = Camera::Inst().GetViewMat();
    const auto& proj = Camera::Inst().GetProjectionMat();
	auto model = _mModelMatrix;

	model = glm::mat4(1);
	model = glm::translate(model, _mTransform.position);
	model = glm::rotate(model, glm::radians(_mTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(_mTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(_mTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "modelMat"), 1, false, (GLfloat*)&model);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "viewMat"), 1, false, (GLfloat*)&view);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "projMat"), 1, false, (GLfloat*)&proj);

	glBindVertexArray(_mVAO);
	glDrawArrays(GL_LINES, 0, 18);

	glBindVertexArray(0);
}

void GameObject::LoadObj(std::string filename)
{
	const auto& paths = Utils::GetResourcePaths();

	printf("Loading: [%s]\n", filename.c_str());

	Assimp::Importer importer;
	for (const std::string& p : paths) {
		std::string fullFilename = p + "/" + filename;

		const aiScene* scene = importer.ReadFile(fullFilename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_FlipUVs);

		if (!scene) continue;

		printf("Loaded:  [%s]\n", fullFilename.c_str());

		std::string dirname = Utils::GetDirname(fullFilename) + "/";

		std::vector<Mesh*> meshes;

		// ProcessNode
		ProcessNode(scene->mRootNode, scene, meshes, dirname);

		AddMesh(meshes[0]);

		return;
	}

	fprintf(stderr, "Failed to load [%s]", filename.c_str());
}

void GameObject::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname)
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

Mesh* GameObject::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname)
{
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<Texture> textures;

	// Indicies
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//vertices.reserve(indices.size());
	//normals.reserve(indices.size());
	//texCoords.reserve(indices.size());
	//tangents.reserve(indices.size());
	//bitangents.reserve(indices.size());

	//for (auto& i : indices)
	//{
	//	auto& temp = vertices[i];
	//	std::vector<glm::vec3> newVert;
	//	newVert.push_back(temp);
	//}

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


		newMat = new Material(amb, diff, spec, aiShininess.r, ambientTex, diffuseTex, specularTex, normalTex);
		newMesh->SetMaterial(newMat);
	}

	return newMesh;
}

std::string GameObject::GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname)
{
	std::string texName;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		texName = dirname + str.C_Str();
	}
	return texName;
}