#include "Texture.h"

Texture::Texture()
{
	_ID = 0;
	width = 0;
	height = 0;
	nrChannels = 0;
}

void Texture::CreateTexture(const std::string& texturePath)
{
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);;

	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAIL_LOAD_TEXTURE" << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

void Texture::CreateTextureA(const std::string& texturePath)
{
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);;

	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAIL_LOAD_TEXTURE" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::ActiveTexture(const unsigned int ID)
{
	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(GL_TEXTURE_2D, _ID);
}

Texture::~Texture()
{
	_ID = 0;
	width = 0;
	height = 0;
	nrChannels = 0;
}
