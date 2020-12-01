#include <cstdio>
#include <Temporality/Temporality.hpp>
#include <Temporality/Module.hpp>
#include <Temporality/Graphics/GraphicsDriver.hpp>
#include <Temporality/Graphics/TextureImporter.hpp>
#include <Temporality/Log.hpp>


class CubeMeshData : Temporality::MeshData {
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
    Temporality::LoadModule("TemporalityOpenGL");
    Temporality::LoadModule("TemporalitySTBI");

    auto gfx = Temporality::GetGraphicsDriver();
    if (gfx) {
        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({"shaders/passThruColor.vert.glsl", "shaders/passThruColor.frag.glsl"});

        auto texture = gfx->CreateTexture();
        texture->LoadFromFile("images/avatars/logo.jpg");

        CubeMeshData meshdata;
        auto mesh = gfx->CreateMesh();
        mesh->Load((Temporality::MeshData*)&meshdata);

        gfx->SetWindowTitle("HelloWorld ~ Temporality");
        gfx->SetWindowSize({ 1024, 768 });
        while (Temporality::IsRunning()) {
            gfx->ProcessEvents();

            shader->Bind();
            mesh->Render();

            gfx->SwapBuffers();
        }
    }

    Temporality::FreeModules();

    return 0;
}