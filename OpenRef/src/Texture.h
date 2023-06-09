#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Texture
{
private:
	GLuint m_ID;
	int m_Width, m_Height, m_BPP;
	unsigned char* m_Buffer;
	std::string m_Path;
public:
	Texture();
	Texture(const char* texturePath);
	~Texture();

	void Bind(GLuint slot = 0);
	void Unbind();

	inline int Width() const { return m_Width; }
	inline int Height() const { return m_Height; }
};

