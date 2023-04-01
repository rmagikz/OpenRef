#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "GLCall.h"
#include <iostream>

class Quad
{
public:
	Quad() {}
	Quad(glm::vec2 position, GLfloat texIndex, const char* path)
	{
		this->Position = position;
		this->TexIndex = texIndex;
		this->Tex = new Texture(path);
		this->Tex->Bind((GLuint)this->TexIndex);

		this->Scale = { this->Tex->Width(), this->Tex->Height()};
		this->AspectRatio = Scale.x / Scale.y;
	}
	glm::vec2 Position = glm::vec2(0);
	glm::vec2 Scale = glm::vec2(0);
	GLfloat TexIndex = 0.0f;
	float AspectRatio = 0.0f;
	Texture* Tex;
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