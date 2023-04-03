#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "GLCall.h"
#include <iostream>

class Quad
{
private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
	glm::vec2 m_ViewOffset;
	glm::vec2 m_ViewScaleOffset;
	GLfloat m_TexIndex;
	float m_AspectRatio;
	Texture* m_Texture;
public:
	Quad() {}
	Quad(glm::vec2 position, GLfloat texIndex, const char* path)
		: m_Position(position), m_Scale(0), m_ViewOffset(0), m_ViewScaleOffset(0), m_TexIndex(texIndex), m_AspectRatio(0), m_Texture()
	{
		m_Texture = new Texture(path);
		m_Texture->Bind((GLuint)m_TexIndex);

		m_Scale = { m_Texture->Width(), m_Texture->Height()};
		m_AspectRatio = m_Scale.x / m_Scale.y;
	}

	inline glm::vec2 FinalPosition() const { return m_Position + m_ViewOffset; }
	inline glm::vec2 FinalScale() const { return m_Scale + m_ViewScaleOffset; }
	inline Texture* GetTexture() const { return m_Texture; }
	inline GLfloat TextureIndex() const { return m_TexIndex; }
	inline float AspectRatio() const { return m_AspectRatio; }
	inline glm::vec2& Position() { return m_Position; }
	inline glm::vec2& Scale() { return m_Scale; }
	inline glm::vec2& ViewOffset() { return m_ViewOffset; }
};

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void Begin();
	static void End();
	static void Flush();

	static void DrawQuad(const Quad& quad);
};