#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GLCall.h"

struct Quad
{
	Quad() {}
	Quad(glm::vec2 position, glm::vec2 scale, GLfloat texIndex)
	{
		this->Position = position;
		this->Scale = scale;
		this->TexIndex = texIndex;
		this->AspectRatio = scale.x / scale.y;
	}
	glm::vec2 Position;
	glm::vec2 Scale;
	GLfloat TexIndex;
	float AspectRatio;
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