#include <Temporality/Temporality.hpp>

#include <cstdio>
#include <memory>

using namespace Temporality;

class CubeMeshData : MeshData {
public:
    CubeMeshData() = default;

    inline Mode GetMode() const
    {
        return MeshData::Mode::Triangles;
    }

    inline gsl::span<const unsigned int> GetIndices() const 
    {
        return gsl::span<const unsigned int>();
    }

    inline gsl::span<const float> GetVertices() const 
    {
        static const float vertices[] = {
            -1.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f,
             0.0f,  1.0f, 0.0f, 1.0f,
        };

        return gsl::span<const float>(vertices, sizeof(vertices) / sizeof(float));
    }

    inline gsl::span<const float> GetNormals() const 
    {
        return gsl::span<const float>();
    }

    inline gsl::span<const float> GetUVs() const
    {
        return gsl::span<const float>();
    }

    inline gsl::span<const float> GetColors() const 
    {
        return gsl::span<const float>();
    }

    inline gsl::span<const float> GetTangents() const 
    {
        return gsl::span<const float>();
    }

    inline gsl::span<const float> GetBitangets() const 
    {
        return gsl::span<const float>();
    }

    inline gsl::span<const unsigned short> GetJoints() const 
    {
        return gsl::span<const unsigned short>();
    }

    inline gsl::span<const float> GetWeights() const 
    {
        return gsl::span<const float>();
    }
};

int main(int argc, char ** argv)
{
    LoadModule("TemporalityOpenGL");
    LoadModule("TemporalitySTBI");
    LoadModule("TemporalityTinyOBJ");

    auto gfx = GetGraphicsDriver();
    if (gfx) {
        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({"shaders/passThruColor.vert.glsl", "shaders/passThruColor.frag.glsl"});

        auto texture = gfx->CreateTexture();
        texture->LoadFromFile("images/avatars/logo.jpg");

        CubeMeshData meshdata;
        auto mesh = gfx->CreateMesh();
        mesh->Load((MeshData*)&meshdata);

        Entity e;

        auto meshComp = e.AddComponent<MeshComponent>(std::unique_ptr<Component>(new MeshComponent()));
        meshComp->LoadFromFile("models/ball/model.obj");

        //e.AddComponent());

        RenderContext* rc = new RenderContext();

        gfx->SetWindowTitle("HelloWorld ~ Temporality");
        gfx->SetWindowSize({ 1024, 768 });
        while (IsRunning()) {
            gfx->ProcessEvents();

            shader->Bind();
            //mesh->Render();
            e.Render(rc);

            gfx->SwapBuffers();
        }
    }

    FreeModules();

    return 0;
}