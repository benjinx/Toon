#include "Model.h"

#include "Mesh.h"
#include "Shader.h"

Model::~Model()
{
    for (Mesh* mesh : _mMeshes)
    {
        delete mesh;
    }
}

void Model::Render(GLuint programNum, Shader * shader)
{
    for (Mesh* mesh : _mMeshes)
    {
        mesh->Render(programNum, shader, _mModelMat);
    }
}
