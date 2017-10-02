#ifndef MODEL_H
#define MODEL_H

#include "Common.h"
#include <glm/glm.hpp>

class Mesh;
class Shader;

class Model
{
public:
    Model(){ _mModelMat = glm::translate(_mModelMat, glm::vec3(0.0f, 0.0f, 0.0f)); };
    ~Model();

    void Render(GLuint programNum, Shader * shader);

    void AddMesh(Mesh* mesh) { _mMeshes.push_back(mesh); }

    Mesh* GetMesh(int index = 0) { return _mMeshes[index]; }
	unsigned int GetNumMeshes() { return _mMeshes.size(); }
	
	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetPosition(glm::vec3 position) { _mModelMat = glm::translate(_mModelMat, position); }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetRotation(float amount, glm::vec3 axis) { _mModelMat = glm::rotate(_mModelMat, glm::radians(amount), axis); }
	
	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetScale(glm::vec3 scale) { _mModelMat = glm::scale(_mModelMat, scale); }

private:
    std::vector<Mesh*> _mMeshes;
	glm::mat4 _mModelMat;
};

#endif // MODEL_H
