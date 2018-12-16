#ifndef SHADER_H
#define SHADER_H

#include "Config.hpp"

class Window;

class Shader
{
public:

	Shader() = default;
    Shader(std::initializer_list<std::string> files);
    ~Shader();

    void CheckAttribs();

	void Load(std::initializer_list<std::string> files);
    void Load(std::string vertFilename, std::string fragFilename);

    void Destroy();

	const GLuint& GetShaderID() { return _mID; }

	void Use()
	{
		glUseProgram(_mID);
	}

	/// Uniform Sets
	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(_mID, name.c_str()), (int)value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(_mID, name.c_str()), value);
	}

	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(_mID, name.c_str()), value);
	}

	void SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(_mID, name.c_str()), 1, &value[0]);
	}

	void SetVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(_mID, name.c_str()), x, y);
	}

	void SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(_mID, name.c_str()), 1, &value[0]);
	}

	void SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(_mID, name.c_str()), x, y, z);
	}

	void SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(_mID, name.c_str()), 1, &value[0]);
	}

	void SetVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(_mID, name.c_str()), x, y, z, w);
	}

	void SetMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(_mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(_mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(_mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:

	GLuint _mID;

};

#endif // SHADER_H
