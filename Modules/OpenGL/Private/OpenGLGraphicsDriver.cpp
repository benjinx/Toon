#include <Toon/OpenGL/OpenGLGraphicsDriver.hpp>

#include <Toon/Toon.hpp>
#include <Toon/Log.hpp>
#include <Toon/Scene.hpp>

namespace Toon::OpenGL {

TOON_OPENGL_API
bool OpenGLGraphicsDriver::Initialize()
{
    if (!SDL2GraphicsDriver::Initialize()) {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    #if defined(TOON_BUILD_DEBUG)

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    #endif

    if (!SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_OPENGL)) {
        return false;
    }

    _glContext = SDL_GL_CreateContext(GetSDL2Window());
    if (!_glContext) {
        ToonLogError("Failed to create OpenGL context, %s", SDL_GetError());
        return false;
    }

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        ToonLogError("Failed to initialize OpenGL context");
        return false;
    }

    #if defined(TOON_BUILD_DEBUG)

        InitDebugMessageCallback();

    #endif

    SDL_GL_SetSwapInterval(0);
    
    ToonLogVerbose("OpenGL Version: %s",  glGetString(GL_VERSION));
    ToonLogVerbose("GLSL Version: %s",    glGetString(GL_SHADING_LANGUAGE_VERSION));
    ToonLogVerbose("OpenGL Vendor: %s",   glGetString(GL_VENDOR));
    ToonLogVerbose("OpenGL Renderer: %s", glGetString(GL_RENDERER));

    int value = 0;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    ToonLogVerbose("Max Vertex Attributes: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &value);
    ToonLogVerbose("Max Vertex Uniform Components: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &value);
    ToonLogVerbose("Max Vertex Output Components: %d", value);

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
    ToonLogVerbose("Max Fragment Uniform Components: %d", value);

    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &value);
    ToonLogVerbose("Max Fragment Input Components: %d", value);

    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &value);
    ToonLogVerbose("Max Draw Buffers: %d", value);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    ToonLogVerbose("Max Fragment Texture Image Units: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &value);
    ToonLogVerbose("Max Vertex Texture Image Units: %d", value);
    
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
    ToonLogVerbose("Max Uniform Buffer Bindings: %d", value);
    
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &value);
    ToonLogVerbose("Max Vertex Uniform Blocks: %d", value);
    
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &value);
    ToonLogVerbose("Max Fragment Uniform Blocks: %d", value);
    
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
    ToonLogVerbose("Max Uniform Block Size: %d", value);

    InitializeUpdateContext();
    InitializeRenderContext();

    if (!InitializeConstantBuffers()) {
        return false;
    }

    if (!InitializeDefaults()) {
        return false;
    }

    return true;
}

TOON_OPENGL_API
void OpenGLGraphicsDriver::Terminate()
{
    if (_glContext) {
        SDL_GL_DeleteContext(_glContext);
        _glContext = nullptr;
    }

    SDL2GraphicsDriver::Terminate();
}

TOON_OPENGL_API
void OpenGLGraphicsDriver::Render()
{
    glm::vec4 cc = GetClearColor();
    glClearColor(cc[0], cc[1], cc[2], cc[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GraphicsDriver::Render();

    uint8_t * data = reinterpret_cast<uint8_t *>(_renderContext->GetShaderGlobals());
    _shaderGlobalsBuffer->WriteTo(0, sizeof(ShaderGlobals), data);

    OpenGLBuffer * glGlobalsBuffer = TOON_OPENGL_BUFFER(_shaderGlobalsBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, TOON_SHADER_GLOBALS_BINDING, glGlobalsBuffer->GetGLID());

    Scene * scene = GetCurrentScene();
    if (scene) {
        scene->Render(_renderContext.get());
    }

    SDL_GL_SwapWindow(GetSDL2Window());
}

TOON_OPENGL_API
std::shared_ptr<Buffer> OpenGLGraphicsDriver::CreateBuffer()
{
    return std::shared_ptr<Buffer>(new OpenGLBuffer());
}

TOON_OPENGL_API
std::shared_ptr<Pipeline> OpenGLGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader)
{
    auto ptr = std::shared_ptr<Pipeline>(new OpenGLPipeline());
    ptr->SetShader(shader);
    return ptr;
}

TOON_OPENGL_API
std::shared_ptr<Texture> OpenGLGraphicsDriver::CreateTexture()
{
    return std::shared_ptr<Texture>(new OpenGLTexture());
}

TOON_OPENGL_API
std::shared_ptr<Shader> OpenGLGraphicsDriver::CreateShader()
{
    auto shader = std::shared_ptr<Shader>(new OpenGLShader());
    _shaderList.push_back(shader);
    return shader;
}

TOON_OPENGL_API
std::shared_ptr<Mesh> OpenGLGraphicsDriver::CreateMesh()
{
    auto ptr = std::shared_ptr<Mesh>(new OpenGLMesh());
    ptr->Initialize();
    return ptr;
}

TOON_OPENGL_API
std::shared_ptr<Material> OpenGLGraphicsDriver::CreateMaterial()
{
    return std::shared_ptr<Material>(new OpenGLMaterial());
}

TOON_OPENGL_API
std::shared_ptr<Primitive> OpenGLGraphicsDriver::CreatePrimitive()
{
    return std::shared_ptr<Primitive>(new OpenGLPrimitive());
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

        #if defined(TOON_BUILD_DEBUG)

            case GL_DEBUG_SEVERITY_NOTIFICATION:
                //if (IsVerboseLoggingEnabled()) {
                    Log(LogLevel::Verbose, "[VERB](OpenGLDebugMessage) %s\n", message);
                //}
                break;
                
        #endif

        }
    }
}

TOON_OPENGL_API
void OpenGLGraphicsDriver::InitDebugMessageCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(_OpenGLDebugMessageCallback, nullptr);
}

} // namespace Toon::OpenGL
