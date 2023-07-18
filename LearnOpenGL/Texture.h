#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();

	void CreateTexture(const std::string& vertexShaderPath);
	void CreateTextureA(const std::string& texturePath);

	void ActiveTexture(const unsigned int ID);

	~Texture();
private:
	unsigned int _ID;
	int width, height, nrChannels;
};

