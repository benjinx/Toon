#ifndef MODEL_H
#define MODEL_H

#include "Common.h"

class Mesh;
class Shader;

class Model
{
public:
    Model(){};
    ~Model();

    void Render(GLuint programNum, Shader * shader);

    void AddMesh(Mesh* mesh) { _mMeshes.push_back(mesh); }

    Mesh* GetMesh(int index) { return _mMeshes[index]; }
	unsigned int GetNumMeshes() { return _mMeshes.size(); }

private:
    std::vector<Mesh*> _mMeshes;
};

#endif // MODEL_H
