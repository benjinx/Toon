#ifndef SHADER_H
#define SHADER_H

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <unordered_map>
#include <vector>

class Shader
{
public:

	Shader() = default;
    Shader(std::initializer_list<std::string> files);
    ~Shader();

    void CheckAttribs();

	void Load(std::initializer_list<std::string> files);
	void Load(std::vector<std::string> files);
    
	void CacheUniforms();
	GLint GetUniformLocation(const std::string& name) const
	{
		if (_mUniforms.find(name) == _mUniforms.end()) return -1;
		return _mUniforms.at(name);
	}

	void Reload();

    void Destroy();

	const GLuint& GetShaderID() { return _mID; }

	void Use()
	{
		glUseProgram(_mID);
	}

	/// Uniform Sets
	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(GetUniformLocation(name), 1, &value[0]);
	}

	void SetVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(GetUniformLocation(name), x, y);
	}

	void SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(GetUniformLocation(name), 1, &value[0]);
	}

	void SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(GetUniformLocation(name), x, y, z);
	}

	void SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(GetUniformLocation(name), 1, &value[0]);
	}

	void SetVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}

	void SetMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

private:

	GLuint _mID;
	std::vector<std::string> _mFiles;
	std::unordered_map<std::string, GLuint> _mUniforms;
};

#endif // SHADER_H
