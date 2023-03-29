#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertPath, const char* fragPath)
	: m_ID(glCreateProgram())
{
	std::string vertSource = ReadShader(vertPath);
	GLuint vert = CompileShader(GL_VERTEX_SHADER, vertSource.c_str());

	std::string fragSource = ReadShader(fragPath);
	GLuint frag = CompileShader(GL_FRAGMENT_SHADER, fragSource.c_str());
	
	GLCall(glAttachShader(m_ID, vert));
	GLCall(glAttachShader(m_ID, frag));

	GLCall(glDeleteShader(vert));
	GLCall(glDeleteShader(frag));

	GLCall(glLinkProgram(m_ID));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ID));
}

void Shader::Bind()
{
	GLCall(glUseProgram(m_ID));
}

void Shader::Unbind()
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const char* name, float v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform2f(const char* name, float v0, float v1)
{
	glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform3f(const char* name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2 ,v3);
}

void Shader::SetUniform4m(const char* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

GLint Shader::GetUniformLocation(const char* name)
{
	if (GLint uniform = glGetUniformLocation(m_ID, name) == -1)
	{
		std::cout << "Uniform " << name << " was not found." << std::endl;
	}
	else
	{
		return uniform;
	}
}

std::string Shader::ReadShader(const char* filePath)
{
	std::ifstream file;
	std::string line;
	std::stringstream content;

	file.open(filePath);

	while (std::getline(file, line))
	{
		content << line << "\n";
	}

	file.close();

	return content.str();
}

GLuint Shader::CompileShader(GLenum shaderType, const char* shaderSource)
{
	GLuint shader = glCreateShader(shaderType);
	GLCall(glShaderSource(shader, 1, &shaderSource, NULL));
	GLCall(glCompileShader(shader));

	ValidateShader(shader);

	return shader;
}

void Shader::ValidateShader(GLuint shader)
{
	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, length, &length, message));
		std::cout << message << std::endl;
	}
}
