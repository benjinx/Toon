#ifndef SHADER_H
#define SHADER_H

#include "Common.hpp"

class Window;

class Shader
{
public:
    Shader(){};
    ~Shader(){};

    void CheckAttribs();

    void SetupShaders(std::string vertFilename, std::string fragFilename);

    void Destroy();

    const std::vector<GLuint>& GetShaderProgramIDs() { return _mShaderProgram; }

private:
    Window*             _mpWindow;
    std::vector<GLuint> _mShaderProgram;
};

#endif // SHADER_H
