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

	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, _ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAIL_LOAD_TEXTURE" << std::endl;
		return;
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
