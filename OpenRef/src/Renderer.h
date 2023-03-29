#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GLCall.h"

struct Quad
{
	glm::vec2 Position;
	glm::vec2 Scale;
	GLfloat TexIndex;
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