#include "Renderer.h"

#include "GLCall.h"

#include <iostream>

static const size_t MaxQuadCount = 32;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
static const size_t MaxTextureSlots = 32;


struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoord;
	GLfloat texIndex;
};

struct RendererData
{
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint IBO = 0;

	Vertex* QuadBuffer = nullptr;
	Vertex* QuadBufferPtr = nullptr;

	uint32_t IndexCount = 0;
} static rData;

void Renderer::Init()
{
	if (rData.QuadBuffer != nullptr) return;

	rData.QuadBuffer = new Vertex[MaxVertexCount];

	GLCall(glCreateVertexArrays(1, &rData.VAO));
	GLCall(glBindVertexArray(rData.VAO));

	GLCall(glGenBuffers(1, &rData.VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rData.VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexArrayAttrib(rData.VAO, 0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));

	GLCall(glEnableVertexArrayAttrib(rData.VAO, 1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord)));

	GLCall(glEnableVertexArrayAttrib(rData.VAO, 2));
	GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex)));

	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;

	for (int i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 0 + offset;
		indices[i + 4] = 2 + offset;
		indices[i + 5] = 3 + offset;

		offset += 4;
	}

	GLCall(glGenBuffers(1, &rData.IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rData.IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

void Renderer::Shutdown()
{
	GLCall(glDeleteVertexArrays(1, &rData.VAO));
	GLCall(glDeleteBuffers(1, &rData.VBO));
	GLCall(glDeleteBuffers(1, &rData.IBO));

	delete[] rData.QuadBuffer;
}

void Renderer::Begin()
{
	rData.QuadBufferPtr = rData.QuadBuffer;
}

void Renderer::End()
{
	GLsizei size = (uint8_t*)rData.QuadBufferPtr - (uint8_t*)rData.QuadBuffer;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rData.VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, rData.QuadBuffer));
}

void Renderer::Flush()
{
	GLCall(glBindVertexArray(rData.VAO));
	GLCall(glDrawElements(GL_TRIANGLES, rData.IndexCount, GL_UNSIGNED_INT, nullptr));

	rData.IndexCount = 0;
}


void Renderer::DrawQuad(const Quad& quad)
{
	if (rData.IndexCount > MaxIndexCount)
	{
		End();
		Flush();
		Begin();
	}

	rData.QuadBufferPtr->position = { quad.Position.x - quad.Scale.x * 0.5f, quad.Position.y + quad.Scale.y * 0.5f };
	rData.QuadBufferPtr->texCoord = { 0.0f, 0.0f };
	rData.QuadBufferPtr->texIndex = quad.TexIndex;
	rData.QuadBufferPtr++;

	rData.QuadBufferPtr->position = { quad.Position.x + quad.Scale.x * 0.5f, quad.Position.y + quad.Scale.y * 0.5f };
	rData.QuadBufferPtr->texCoord = { 1.0f, 0.0f };
	rData.QuadBufferPtr->texIndex = quad.TexIndex;
	rData.QuadBufferPtr++;

	rData.QuadBufferPtr->position = { quad.Position.x + quad.Scale.x * 0.5f, quad.Position.y - quad.Scale.y * 0.5f };
	rData.QuadBufferPtr->texCoord = { 1.0f, 1.0f };
	rData.QuadBufferPtr->texIndex = quad.TexIndex;
	rData.QuadBufferPtr++;

	rData.QuadBufferPtr->position = { quad.Position.x - quad.Scale.x * 0.5f, quad.Position.y - quad.Scale.y * 0.5f };
	rData.QuadBufferPtr->texCoord = { 0.0f, 1.0f };
	rData.QuadBufferPtr->texIndex = quad.TexIndex;
	rData.QuadBufferPtr++;

	rData.IndexCount += 6;
}
