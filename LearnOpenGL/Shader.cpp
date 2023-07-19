#include "Shader.h"

Shader::Shader()
{
	_ID = 0;
}

void Shader::CreateFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::string vertexCode = ReadFile(vertexShaderPath);
	std::string fragmentCode = ReadFile(fragmentShaderPath);

	_ID = glCreateProgram();
	AttachShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	AttachShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
	glLinkProgram(_ID);

	int success;
	char infoLog[512];
	glGetProgramiv(_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM_LINK_FAILED\n" << infoLog << std::endl;
		return;
	}
}

std::string Shader::ReadFile(const std::string& path)
{
	std::string content;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	try
	{
		fileStream.open(path);
		std::stringstream shaderStream;
		shaderStream << fileStream.rdbuf();

		fileStream.close();

		content = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	return content;
}

void Shader::AttachShader(const char* shaderCode, GLenum shaderType)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	glAttachShader(_ID, shader);
	glDeleteShader(shader);
}

void Shader::UseShader()
{
	glUseProgram(_ID);
}

unsigned int Shader::GetID()
{
	return _ID;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	int uniformLocation = glGetUniformLocation(_ID, name.c_str());
	glUniform1i(uniformLocation, (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	int uniformLocation = glGetUniformLocation(_ID, name.c_str());
	glUniform1i(uniformLocation, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	int uniformLocation = glGetUniformLocation(_ID, name.c_str());
	glUniform1f(uniformLocation, value);
}

void Shader::ClearShader()
{
	if (_ID != 0)
	{
		glDeleteProgram(_ID);
		_ID = 0;
	}
}

Shader::~Shader()
{
	ClearShader();
}