#include "Shader.h"

#include "Window.h"
#include "Utils.h"

void Shader::CheckAttribs()
{
    // Check for max Attribs
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

void Shader::SetupShaders(std::string vertFilename, std::string fragFilename)
{
	#pragma region Setup Shaders
	//			// setup and delete shaders
	//void setupShaders()
	//{
	//	unsigned int u = 0;
	//	egpFileInfo files[4];
	//	egpShader shaders[4];
	//
	//	// array of common uniform names
	//	const char *commonUniformName[] = {
	//		(const char *)("mvp"),
	//		(const char *)("lightPos"),
	//		(const char *)("eyePos"),
	//		(const char *)("tex_dm"),
	//		(const char *)("tex_sm"),
	//		(const char *)("tex_nm"),
	//		(const char *)("tex_hm"),
	//		(const char *)("pixelSizeInv"),
	//		(const char *)("img"),
	//		(const char *)("img1"),
	//		(const char *)("img2"),
	//		(const char *)("img3"),
	//	};
	//
	//	const int imageLocations[] = {
	//		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	//	};
	//
	//
	//	// activate a VAO for validation
	//	egpActivateVAO(vao + sphere8x6Model);
	//
	//
	//	// test color program
	//	{
	//		// load files
	//		files[0] = egpLoadFileContents("../../../../resource/glsl/4x/vs/passColor_vs4x.glsl");
	//		files[1] = egpLoadFileContents("../../../../resource/glsl/4x/fs/drawColor_fs4x.glsl");
	//
	//		// create shaders
	//		shaders[0] = egpCreateShaderFromSource(EGP_SHADER_VERTEX, files[0].contents);
	//		shaders[1] = egpCreateShaderFromSource(EGP_SHADER_FRAGMENT, files[1].contents);
	//
	//		currentProgramIndex = testColorProgramIndex;
	//		currentProgram = glslPrograms + currentProgramIndex;
	//
	//		// create, link and validate program
	//		*currentProgram = egpCreateProgram();
	//		egpAttachShaderToProgram(currentProgram, shaders + 0);
	//		egpAttachShaderToProgram(currentProgram, shaders + 1);
	//		egpLinkProgram(currentProgram);
	//		egpValidateProgram(currentProgram);
	//
	//		// release shaders and files
	//		egpReleaseShader(shaders + 0);
	//		egpReleaseShader(shaders + 1);
	//		egpReleaseFileContents(files + 0);
	//		egpReleaseFileContents(files + 1);
	//	}
	//
	//	// configure all uniforms at once
	//	for (currentProgramIndex = 0; currentProgramIndex < GLSLProgramCount; ++currentProgramIndex)
	//	{
	//		// get location of every uniform
	//		currentProgram = glslPrograms + currentProgramIndex;
	//		currentUniformSet = glslCommonUniforms[currentProgramIndex];
	//		egpActivateProgram(currentProgram);
	//		for (u = 0; u < GLSLCommonUniformCount; ++u)
	//			currentUniformSet[u] = egpGetUniformLocation(currentProgram, commonUniformName[u]);
	//
	//		// bind constant uniform locations, if they exist, because they never change
	//		// e.g. image bindings
	//		egpSendUniformInt(currentUniformSet[unif_dm], UNIF_INT, 1, imageLocations);
	//		egpSendUniformInt(currentUniformSet[unif_sm], UNIF_INT, 1, imageLocations + 1);
	//		egpSendUniformInt(currentUniformSet[unif_nm], UNIF_INT, 1, imageLocations + 2);
	//		egpSendUniformInt(currentUniformSet[unif_hm], UNIF_INT, 1, imageLocations + 3);
	//
	//		egpSendUniformInt(currentUniformSet[unif_img], UNIF_INT, 1, imageLocations);
	//		egpSendUniformInt(currentUniformSet[unif_img1], UNIF_INT, 1, imageLocations + 1);
	//		egpSendUniformInt(currentUniformSet[unif_img2], UNIF_INT, 1, imageLocations + 2);
	//		egpSendUniformInt(currentUniformSet[unif_img3], UNIF_INT, 1, imageLocations + 3);
	//	}
	//
	//
	//	// disable all
	//	egpActivateProgram(0);
	//	egpActivateVAO(0);
	//}
#pragma endregion

    // Load Shaders
    // Retrieve the vertex/fragment source code from filePath
    std::string   vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    // Ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open files
        vShaderFile.open(vertFilename);
        fShaderFile.open(fragFilename);

        std::stringstream vShaderStream, fShaderStream;

        // Read files buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into GLchar array
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY" << std::endl;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

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

    GLuint shaderProgramID = glCreateProgram();
    _mShaderProgram.push_back(shaderProgramID);

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Destroy()
{
	for (auto program : _mShaderProgram)
	{
		glUseProgram(program);
		glDeleteProgram(program);
	}

	_mShaderProgram.clear();
}