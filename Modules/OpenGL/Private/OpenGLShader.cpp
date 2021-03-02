#include <Toon/OpenGL/OpenGLShader.hpp>
#include <Toon/Log.hpp>
#include <Toon/Util.hpp>
#include <Toon/Benchmark.hpp>

#include <fstream>
#include <vector>
#include <sstream>
#include <functional>

namespace Toon::OpenGL {

TOON_OPENGL_API
bool OpenGLShader::LoadFromFiles(const std::vector<std::string>& filenames, bool useAssetPath /*= true*/)
{
    ToonBenchmarkStart();

    std::vector<GLuint> shaders;

    for (const auto& filename : filenames) {
        GLuint shader = 0;

        const string& ext = GetExtension(filename);

        if (ext == "spv") {
            shader = LoadSPV(filename, useAssetPath);
        }
        else if (ext == "glsl") {
            shader = LoadGLSL(filename, useAssetPath);
        }
        else {
            shader = LoadSPV(filename + ".spv", useAssetPath);
            if (shader == 0) {
                shader = LoadGLSL(filename + ".glsl", useAssetPath);
            }
        }

        if (shader == 0) {
            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }

            ToonLogError("Failed to load '%s'", filename);
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

            ToonLogError("Failed to compile shader '%s'\n%s", filename, log.data());
            return false;
        }

        shaders.push_back(shader);
    }

    _glID = glCreateProgram();
    if (!_glID) {
        ToonLogError("Failed to create shader program");
        return false;
    }

    for (GLuint shader : shaders) {
        glAttachShader(_glID, shader);
    }

    glLinkProgram(_glID);
    
    GLint linked = GL_FALSE;
    glGetProgramiv(_glID, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logLength = 0;
        glGetProgramiv(_glID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(_glID, logLength, &logLength, log.data());

        glDeleteProgram(_glID);
        _glID = 0;

        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }

        ToonLogError("Failed to link shader\n%s", log.data());
        return false;
    }

    for (GLuint shader : shaders) {
        glDetachShader(_glID, shader);
    }

    ToonBenchmarkEnd();
    return true;
}

TOON_OPENGL_API
void OpenGLShader::Bind()
{
    glUseProgram(_glID);
}

TOON_OPENGL_API
GLuint OpenGLShader::GetID()
{
    return _glID;
}

TOON_OPENGL_API
GLuint OpenGLShader::LoadSPV(const std::string& filename, bool useAssetPath)
{
    ToonLogVerbose("Looking for SPIR-V shader '%s'", filename);

    const auto& assetPathList = GetAssetPathList();

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& path : assetPathList) {
            Path fullPath = path / "Shaders" / filename;
            ToonLogVerbose("Checking '%s'", fullPath);
            
            file.open(fullPath.ToString(), std::ios::binary);
            if (file.is_open()) {
                break;
            }
        }
    }
    else {
        file.open(filename, std::ios::binary);
    }

    if (!file.is_open()) {
        return 0;
    }

    file.unsetf(std::ios::skipws);

    ToonLogLoad("Loading SPIR-V shader '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    GLenum type = GetGLShaderType(filename);
    if (type == GL_INVALID_ENUM) {
        ToonLogError("Failed to determine shader type of '%s'", filename);
        return 0;
    }

    GLuint shader = glCreateShader(type);

    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());

    glSpecializeShader(shader, "main", 0, nullptr, nullptr);

    return shader;
}

TOON_OPENGL_API
GLuint OpenGLShader::LoadGLSL(const std::string& filename, bool useAssetPath)
{
    ToonLogVerbose("Looking for GLSL shader '%s'", filename);

    const auto& assetPaths = GetAssetPathList();

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& path : assetPaths) {
            Path fullPath = path / "Shaders" / filename;
            ToonLogVerbose("Checking '%s'", fullPath);
            
            file.open(fullPath.ToString());
            if (file.is_open()) {
                break;
            }
        }
    }
    else {
        file.open(filename);
    }

    if (!file.is_open()) {
        return 0;
    }
    
    ToonLogLoad("Loading GLSL shader '%s'", filename);

    string code(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    // TODO: Support relative includes, without asset path

    std::function<string(string)> preprocessGLSL = 
        [&](string code) -> string {
            std::istringstream iss(code);
            string outCode;
            string line;

            while (std::getline(iss, line)) {
                if (line.compare(0, sizeof("#include")-1, "#include") == 0) {
                    size_t left = line.find('"');
                    size_t right = line.rfind('"');

                    if (left == string::npos || right == string::npos) {
                        left = line.find('<');
                        right = line.rfind('>');

                        if (left == string::npos || right == string::npos) {
                            ToonLogError("Unable to parse filename from shader include");
                            return "";
                        }
                    }

                    string incFilename = line.substr(left + 1, right - left - 1);
                    std::ifstream incFile;

                    for (const auto& path : assetPaths) {
                        Path fullPath = path / "Shaders" / incFilename;
                        ToonLogVerbose("Checking '%s'", fullPath);
                        
                        incFile.open(fullPath);
                        if (incFile.is_open()) {
                            break;
                        }
                    }

                    if (!incFile.is_open()) {
                        ToonLogError("Unable to find shader include '%s'", incFilename);
                        return "";
                    }

                    string incCode(
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
        ToonLogError("Unable to determine shader type of '%s'", filename);
        return 0;
    }

    GLuint shader = glCreateShader(type);
    
    const char * ptr = code.c_str();
    glShaderSource(shader, 1, (const GLchar **)&ptr, nullptr);

    glCompileShader(shader);

    return shader;
}

TOON_OPENGL_API
GLenum OpenGLShader::GetGLShaderType(const std::string& filename)
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

} // namespace Toon::OpenGL