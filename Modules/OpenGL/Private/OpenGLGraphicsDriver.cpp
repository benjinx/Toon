#include <Temporality/OpenGL/OpenGLGraphicsDriver.hpp>
#include <Temporality/Log.hpp>

#include <glad/gl.h>

#include <Temporality/Temporality.hpp>
#include <Temporality/OpenGL/OpenGLMesh.hpp>
#include <Temporality/OpenGL/OpenGLShader.hpp>
#include <Temporality/OpenGL/OpenGLTexture.hpp>

namespace Temporality::OpenGL {

TEMPORALITY_OPENGL_API
bool OpenGLGraphicsDriver::Initialize()
{
    if (!SDL2GraphicsDriver::Initialize()) {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    #if defined(TEMPORALITY_BUILD_DEBUG)

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    #endif

    LogInfo("Creating SDL2 Window.");

    if (!SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)) {
        return false;
    }

    LogInfo("SDL2 Window Created Successfully.");

    LogInfo("Creating SDL2 GL Context.");

    _glContext = SDL_GL_CreateContext(GetSDL2Window());

    if (!_glContext)
    {
        LogError("Failed to create SDL2 GL Context. %s", SDL_GetError());
        return false;
    }

    LogLoad("SDL2 GL Context created successfully.\n");

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        LogError("Failed to initialize OpenGL context.");
        return false;
    }

    #if defined(TEMPORALITY_BUILD_DEBUG)

        InitDebugMessageCallback();

    #endif

    LogVerbose("OpenGL Version: %s",  glGetString(GL_VERSION));
    LogVerbose("GLSL Version: %s",    glGetString(GL_SHADING_LANGUAGE_VERSION));
    LogVerbose("OpenGL Vendor: %s",   glGetString(GL_VENDOR));
    LogVerbose("OpenGL Renderer: %s", glGetString(GL_RENDERER));

    int value = 0;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    LogVerbose("Max Vertex Attributes: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &value);
    LogVerbose("Max Vertex Uniform Components: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &value);
    LogVerbose("Max Vertex Output Components: %d", value);

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
    LogVerbose("Max Fragment Uniform Components: %d", value);

    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &value);
    LogVerbose("Max Fragment Input Components: %d", value);

    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &value);
    LogVerbose("Max Draw Buffers: %d", value);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    LogVerbose("Max Fragment Texture Image Units: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &value);
    LogVerbose("Max Vertex Texture Image Units: %d", value);
    
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
    LogVerbose("Max Uniform Buffer Bindings: %d", value);
    
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &value);
    LogVerbose("Max Vertex Uniform Blocks: %d", value);
    
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &value);
    LogVerbose("Max Fragment Uniform Blocks: %d", value);
    
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
    LogVerbose("Max Uniform Block Size: %d", value);

    // TODO: Move
    const unsigned TRANSFORM_DATA_BINDING = 0;

    bool result;

    std::shared_ptr<Buffer> transformDataBuffer = std::shared_ptr<Buffer>(new OpenGLBuffer());
    result = transformDataBuffer->Initialize(
        sizeof(TransformData),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    if (!result) {
        LogError("Freak out");
    }

    AddConstantBuffer(transformDataBuffer, TRANSFORM_DATA_BINDING);

    return true;
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::Terminate()
{
    if (_glContext) {
        SDL_GL_DeleteContext(_glContext);
        _glContext = nullptr;
    }

    SDL2GraphicsDriver::Terminate();
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::Render()
{
    glm::vec4 cc = GetClearColor();
    glClearColor(cc[0], cc[1], cc[2], cc[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    BindUniformBufferObjects();

    if (_currentScene) {
        _currentScene->Render(_renderContext.get());
    }

    SDL_GL_SwapWindow(GetSDL2Window());
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Pipeline> OpenGLGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader)
{
    auto ptr = std::shared_ptr<Pipeline>(new OpenGLPipeline());
    ptr->SetShader(shader);
    return ptr;
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Texture> OpenGLGraphicsDriver::CreateTexture()
{
    return std::shared_ptr<Texture>(new OpenGLTexture());
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Shader> OpenGLGraphicsDriver::CreateShader()
{
    auto shader = std::shared_ptr<Shader>(new OpenGLShader());
    _shaders.push_back(shader);
    return shader;
}

TEMPORALITY_OPENGL_API
std::shared_ptr<Mesh> OpenGLGraphicsDriver::CreateMesh()
{
    return std::shared_ptr<Mesh>(new OpenGLMesh());
}

TEMPORALITY_OPENGL_API
std::unique_ptr<Primitive> OpenGLGraphicsDriver::CreatePrimitive()
{
    return std::unique_ptr<Primitive>(new OpenGLPrimitive());
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::BindUniformBufferObjects()
{
    for (const auto& it : _constantBufferBindings) {
        OpenGLBuffer * buffer = TEMPORALITY_OPENGL_BUFFER(it.second.get());
        if (!buffer) {
            continue;
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, it.first, buffer->GetGLID());
    }
}

void GLAPIENTRY
_OpenGLDebugMessageCallback(
    GLenum source, GLenum type, GLenum id, GLenum severity,
    GLsizei length, const GLchar * message, const void * userData)
{
    if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        Log(LogLevel::Performance, "[PERF](OpenGLDebugMessage) %s\n", message);
    }
    else {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log(LogLevel::Error, "[ERRO](OpenGLDebugMessage) %s\n", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log(LogLevel::Warning, "[WARN](OpenGLDebugMessage) %s\n", message);
            break;

        #if defined(TEMPORALITY_ENABLE_VERBOSE_LOGGING)

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            Log(LogLevel::Verbose, "[VERB](OpenGLDebugMessage) %s\n", message);
            break;

        #endif
        
        }
    }
}

TEMPORALITY_OPENGL_API
void OpenGLGraphicsDriver::InitDebugMessageCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(_OpenGLDebugMessageCallback, nullptr);
}

} // namespace Temporality::OpenGL