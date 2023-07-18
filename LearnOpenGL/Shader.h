#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader();

	void CreateFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	std::string ReadFile(const std::string& path);

	void AttachShader(const char* shaderCode, GLenum shaderType);

	void UseShader();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	void ClearShader();

	~Shader();
private:
	unsigned int _program;
};