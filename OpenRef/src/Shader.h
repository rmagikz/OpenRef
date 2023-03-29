#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "GLCall.h"

#include <string>

class Shader
{
private:
	GLuint m_ID;
public:
	Shader(const char* vertPath, const char* fragPath);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniform1f(const char* name, float v0);
	void SetUniform2f(const char* name, float v0, float v1);
	void SetUniform3f(const char* name, float v0, float v1, float v2);
	void SetUniform4f(const char* name, float v0 ,float v1, float v2, float v3);
	void SetUniform4m(const char* name, const glm::mat4& matrix);
private:
	GLint GetUniformLocation(const char* name);
	std::string ReadShader(const char* filePath);
	GLuint CompileShader(GLenum shaderType, const char* shaderSource);
	void ValidateShader(GLuint shader);
};