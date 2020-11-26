#include <Temporality/OpenGL/Shader.hpp>
#include <Temporality/Log.hpp>
#include <Temporality/Utils.hpp>

#include <fstream>
#include <vector>
#include <sstream>

namespace Temporality::OpenGL {

TEMPORALITY_OPENGL_API
bool Shader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    BenchmarkStart();

    std::vector<GLuint> shaders;
    const auto& shaderPaths = GetAssetPaths();

    for (const auto& filename : filenames) {
        GLuint shader = 0;

        const std::string& ext = GetExtension(filename);

        if (ext == "spv") {
            shader = LoadSPV(filename);
        }
        else if (ext == "glsl") {
            shader = LoadGLSL(filename);
        }
        else {
            shader = LoadSPV(filename + ".spv");
            if (shader == 0) {
                shader = LoadGLSL(filename + ".glsl");
            }
        }

        if (shader == 0) {
            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }

            LogError("Failed to load '%s'", filename);
            return false;
        }

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, log.data());

            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }

            LogError("Failed to compile shader '%s'\n%s", filename, log.data());
            return false;
        }

        shaders.push_back(shader);
    }

    _mglID = glCreateProgram();
    if (!_mglID) {
        LogError("Failed to create shader program");
        return false;
    }

    for (GLuint shader : shaders) {
        glAttachShader(_mglID, shader);
    }

    glLinkProgram(_mglID);

    GLint linked = GL_FALSE;
    glGetProgramiv(_mglID, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logLength = 0;
        glGetProgramiv(_mglID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(_mglID, logLength, &logLength, log.data());

        glDeleteProgram(_mglID);
        _mglID = 0;

        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }

        LogError("Failed to link shader\n%s", log.data());
        return false;
    }

    for (GLuint shader : shaders) {
        glDetachShader(_mglID, shader);
    }

    BenchmarkEnd("OpenGL::Shader::LoadFromFiles");
    return true;
}

TEMPORALITY_OPENGL_API
void Shader::Bind()
{
    glUseProgram(_mglID);
}

TEMPORALITY_OPENGL_API
GLuint Shader::GetID()
{
    return _mglID;
}

TEMPORALITY_OPENGL_API
GLuint Shader::LoadSPV(const std::string& filename)
{
    LogVerbose("Looking for SPIR-V shader '%s'", filename);

    const auto& shaderPaths = GetAssetPaths();

    std::ifstream file;

    for (const auto& path : shaderPaths) {
        const std::string& fullPath = path + filename;
        LogVerbose("Checking '%s'", fullPath);

        file.open(fullPath, std::ios::binary);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open()) {
        return 0;
    }

    file.unsetf(std::ios::skipws);

    LogLoad("Loading SPIR-V shader '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    GLenum type = GetGLShaderType(filename);
    if (type == GL_INVALID_ENUM) {
        LogError("Failed to deteremine shader type of '%s'", filename);
        return 0;
    }

    GLuint shader = glCreateShader(type);
    
    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());

    glSpecializeShader(shader, "main", 0, nullptr, nullptr);

    return shader;
}

TEMPORALITY_OPENGL_API
GLuint Shader::LoadGLSL(const std::string& filename)
{
    LogVerbose("Looking for GLSL shader '%s'", filename);

    const auto& shaderPaths = GetAssetPaths();

    std::ifstream file;

    for (const auto& path : shaderPaths) {
        LogVerbose("Checking '%s'", path + filename);

        file.open(path + filename);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open()) {
        return 0;
    }

    LogLoad("Loading GLSL shader '%s'", filename);

    std::string code(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    std::function<std::string(std::string)> preprocessGLSL =
        [&](std::string code) -> std::string {
            std::istringstream iss(code);
            std::string outCode;
            std::string line;

            while (std::getline(iss, line)) {
                if (line.compare(0, sizeof("#include")-1, "#include") == 0) {
                    size_t left = line.find('"');
                    size_t right = line.rfind('"');

                    if (left == std::string::npos || right == std::string::npos) {
                        left = line.find('<');
                        right = line.rfind('>');

                        if (left == std::string::npos || right == std::string::npos) {
                            LogError("Unable to parse filename from shader include");
                            return "";
                        }
                    }

                    std::string incFilename = line.substr(left + 1, right - left - 1);
                    std::ifstream incFile;

                    for (const auto& path : shaderPaths) {
                        LogVerbose("Checking '%s'", path + incFilename);

                        incFile.open(path + incFilename);
                        if (incFile.is_open()) {
                            break;
                        }
                    }

                    if (!incFile.is_open()) {
                        LogError("Unable to find shader include '%s'", incFilename);
                        return "";
                    }

                    std::string incCode(
                        (std::istreambuf_iterator<char>(incFile)),
                        std::istreambuf_iterator<char>()
                    );

                    outCode += preprocessGLSL(incCode);
                }
                else {
                    outCode += line + "\n";
                }
            }

            return outCode;
        };

    code = preprocessGLSL(code);

    GLenum type = GetGLShaderType(filename);
    if (type == GL_INVALID_ENUM) {
        LogError("Unable to determine shader type of '%s'", filename);
        return 0;
    }

    GLuint shader = glCreateShader(type);

    const char * ptr = code.c_str();
    glShaderSource(shader, 1, (const GLchar **)&ptr, nullptr);

    glCompileShader(shader);

    return shader;
}

TEMPORALITY_OPENGL_API
GLenum Shader::GetGLShaderType(const std::string& filename)
{
    std::string ext = GetExtension(filename);
    if (ext == "spv" || ext == "glsl") {
        size_t pivot = filename.find_last_of('.');
        if (pivot == std::string::npos) {
            return GL_INVALID_ENUM;
        }
        ext = GetExtension(filename.substr(0, pivot));
    }

    if (ext == "vert" || ext == "vertex") {
        return GL_VERTEX_SHADER;
    }
    else if (ext == "frag" || ext == "fragment") {
        return GL_FRAGMENT_SHADER;
    }
    else if (ext == "tesc" || ext == "tesscontrol") {
        return GL_TESS_CONTROL_SHADER;
    }
    else if (ext == "tese" || ext == "tesseval") {
        return GL_TESS_EVALUATION_SHADER;
    }
    else if (ext == "comp" || ext == "compute") {
        return GL_COMPUTE_SHADER;
    }

    return GL_INVALID_ENUM;
}

} // namespace Temporality::OpenGL