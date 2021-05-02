#include "textureWithClass.h"

Texture::Texture(const std::string& path, bool gammaCorrection)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &(this->width), &(this->height), &(this->nrComponents), 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum format;
		if (this->nrComponents == 1)
		{
			internalFormat = format = GL_RED;
		}
		else if (this->nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			format = GL_RGB;
		}
		else if (this->nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);

	this->textureID = textureID;
}

GLuint Texture::GetTextureID() { return textureID;  }
int Texture::GetWidth() { return width;  }
int Texture::GetHeight() { return height;  }