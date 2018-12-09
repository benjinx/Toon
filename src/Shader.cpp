#include "Shader.hpp"

#include "Window.hpp"
#include "Utils.hpp"

Shader::~Shader() {
    Destroy();
}

void Shader::CheckAttribs()
{
    // Check for max Attribs
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

void Shader::Load(std::string vertFilename, std::string fragFilename)
{
    const auto& paths = Utils::GetResourcePaths();

    printf("Loading: [%s]\n", vertFilename.c_str());
    printf("Loading: [%s]\n", fragFilename.c_str());

    // Load Shaders
    // Retrieve the vertex/fragment source code from filePath
    std::ifstream vertShaderFile;
    std::ifstream fragShaderFile;

    bool loaded = false;
    for (const std::string& p : paths) {
        std::string fullVertFilename = p + "/" + vertFilename;
        std::string fullFragFilename = p + "/" + fragFilename;

        vertShaderFile.open(fullVertFilename);
        fragShaderFile.open(fullFragFilename);

        if (vertShaderFile.is_open() && fragShaderFile.is_open()) {
            printf("Loaded:  [%s]\n", fullVertFilename.c_str());
            printf("Loaded:  [%s]\n", fullFragFilename.c_str());
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        fprintf(stderr, "Failed to load shaders [%s] [%s]\n", vertFilename.c_str(), fragFilename.c_str());
        return;
    }

    std::string vertCode((std::istreambuf_iterator<char>(vertShaderFile)),
                         std::istreambuf_iterator<char>());
    std::string fragCode((std::istreambuf_iterator<char>(fragShaderFile)),
                         std::istreambuf_iterator<char>());

    vertShaderFile.close();
    fragShaderFile.close();

    const GLchar* vShaderCode = vertCode.c_str();
    const GLchar* fShaderCode = fragCode.c_str();

    // Shader & Program Init
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    GLint  success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

	_mID = glCreateProgram();

    glAttachShader(_mID, vertexShader);
    glAttachShader(_mID, fragmentShader);
    glLinkProgram(_mID);

    glGetProgramiv(_mID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(_mID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Destroy()
{
    if (_mID != 0) {
	    glDeleteProgram(_mID);
        _mID = 0;
    }
}
