#include "Texture.h"

#include "GLCall.h"
#include <iostream>

#include "stb/stb_image.h"

Texture::Texture()
	: m_ID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Path("")
{
}

Texture::Texture(const char* texturePath)
	: m_ID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Path(texturePath)
{
	stbi_set_flip_vertically_on_load(1);

	m_Buffer = stbi_load(texturePath, &m_Width, &m_Height, &m_BPP, 4);
	std::cout << "Texture Width and Height: " << m_Width << ", " << m_Height << std::endl;

	GLCall(glGenTextures(1, &m_ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_Buffer)
	{
		stbi_image_free(m_Buffer);
	}
}

Texture::Texture(const Texture& other)
	: m_ID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Path(other.m_Path)
{
	std::cout << "COPIED!!!" << std::endl;
	stbi_set_flip_vertically_on_load(1);

	m_Buffer = stbi_load(other.m_Path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	std::cout << "Texture Width and Height: " << m_Width << ", " << m_Height << std::endl;

	GLCall(glGenTextures(1, &m_ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_Buffer)
	{
		stbi_image_free(m_Buffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_ID));
}

void Texture::Bind(GLuint slot)
{
	//GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTextureUnit(slot, m_ID));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
