#include "Shader.hpp"

#include <Utils.hpp>
#include <Log.hpp>

#include <fstream>

Shader::Shader(std::initializer_list<std::string> files)
{
    Load(files);
}

Shader::~Shader() {
    Destroy();
}

void Shader::CheckAttribs()
{
    // Check for max Attribs
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    LogInfo("Maximum number of vertex attributes supported: ", nrAttributes);
}

void Shader::Load(std::initializer_list<std::string> files)
{
    Load(std::vector<std::string>(files));
}

void Shader::Load(std::vector<std::string> files)
{
    _mFiles = files;

    const auto& paths = Utils::GetResourcePaths();

    std::vector<GLuint> shaderIds;

    GLint success;
    GLchar infoLog[512];

    for (auto& f : files)
    {
        // Output files loading
        LogInfo("Loading: [%s]", f);

        // Find shader type
        GLuint shaderType;
        std::string ext = f.substr(f.find_last_of(".") + 1);
        if (ext == "vert")
        {
            shaderType = GL_VERTEX_SHADER;
        }
        else if (ext == "frag")
        {
            shaderType = GL_FRAGMENT_SHADER;
        }
        else if (ext == "gs")
        {
            shaderType = GL_GEOMETRY_SHADER;
        }
#ifdef GL_VERSION_4_0
        // Requires OpenGL 4.0+
        else if (ext == "tcs")
        {
            shaderType = GL_TESS_CONTROL_SHADER;
        }
        else if (ext == "tes")
        {
            shaderType = GL_TESS_EVALUATION_SHADER;
        }
#endif // GL_VERSION_4_0
#ifdef GL_VERSION_4_3
        else if (ext == "cs")
        {
            shaderType = GL_COMPUTE_SHADER;
        }
#endif // GL_VERSION_4_3

        // Load Shader
        std::ifstream shaderFile;
        bool loaded = false;
        for (auto& p : paths)
        {
            std::string fullFilename = p + "/" + f;

            shaderFile.open(fullFilename);

            if (shaderFile.is_open())
            {
                LogLoad("Loaded:  [%s]", fullFilename);
                loaded = true;
                break;
            }
        }

        if (!loaded)
        {
            LogError("Failed to load shaders [%s]", f);
            return;
        }

        // Create Shader
        std::string shaderCode((std::istreambuf_iterator<char>(shaderFile)),
                                std::istreambuf_iterator<char>());

        // Clean up!
        shaderFile.close();

        // Get Pointer to string
        const GLchar* pShaderCode = shaderCode.c_str();

        // Create shader
        GLuint shader;
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &pShaderCode, NULL);
        glCompileShader(shader);

        // Check status
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            LogError("Failed to compile shaders [%s] \n%s", f, infoLog);
            return;
        }

        // Store Ids
        shaderIds.push_back(shader);
    }

    // Create Program
    _mID = glCreateProgram();

    // Attach shaders to program
    for (auto& id : shaderIds)
    {
        glAttachShader(_mID, id);
    }

    // Link Program
    glLinkProgram(_mID);

    // Check status
    glGetProgramiv(_mID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_mID, sizeof(infoLog), NULL, infoLog);
        LogError("Failed to link shader [%d]", _mID);
        return;
    }

    // Clean up!
    for (auto& id : shaderIds)
    {
        glDeleteShader(id);
    }

    // Cache our uniforms
    CacheUniforms();
}

void Shader::CacheUniforms()
{
    GLint tmpSize; // size of the variable
    GLenum tmpType; // type of the variable (float, vec3 or mat4, etc)

    GLchar buffer[256]; // variable name in GLSL
    GLsizei length; // name length

    GLint count;
    glGetProgramiv(_mID, GL_ACTIVE_UNIFORMS, &count);
    for (GLint i = 0; i < count; ++i)
    {
        glGetActiveUniform(_mID, (GLuint)i, sizeof(buffer), &length, &tmpSize, &tmpType, buffer);
        _mUniforms.emplace(buffer, glGetUniformLocation(_mID, buffer));
    }
}

void Shader::Reload()
{
    glDeleteShader(_mID);
    Load(_mFiles);
}

void Shader::Destroy()
{
    if (_mID != 0) {
        glDeleteProgram(_mID);
        _mID = 0;
    }
}
