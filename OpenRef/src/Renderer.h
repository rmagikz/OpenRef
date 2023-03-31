#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "GLCall.h"
#include <iostream>

struct Quad
{
	Quad() {}
	Quad(glm::vec2 position, glm::vec2 scale, GLfloat texIndex)
	{
		this->Position = position;
		this->TexIndex = texIndex;
		this->Scale = scale;
		this->AspectRatio = scale.x / scale.y;
	}
	glm::vec2 Position = glm::vec2(0);
	glm::vec2 Scale = glm::vec2(0);
	GLfloat TexIndex = 0.0f;
	float AspectRatio = 0.0f;
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