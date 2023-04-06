#include "Quad.h"

Quad::Quad()
	: m_Position(0), m_Scale(0), m_TexIndex(0), m_AspectRatio(0), m_Texture(0)
{
}

Quad::Quad(glm::vec2 position, GLfloat texIndex, const char* path)
	: m_Position(position), m_Scale(0), m_TexIndex(texIndex), m_AspectRatio(0), m_Texture()
{
	m_Texture = new Texture(path);
	m_Texture->Bind((GLuint)m_TexIndex);

	m_Scale = { m_Texture->Width(), m_Texture->Height() };
	m_AspectRatio = m_Scale.x / m_Scale.y;
}

void Quad::Translate(const glm::vec2& translation)
{
	m_Position += translation;
}

void Quad::SetPosition(const glm::vec2& position)
{
	m_Position = position;
}

void Quad::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;
}

void Quad::CalculateAspectRatio()
{
	m_Scale.y = m_Scale.x / m_AspectRatio;
}

void Quad::ClampScale(const glm::vec2& min, const glm::vec2& max)
{
	m_Scale = glm::clamp(m_Scale, min, max);
}

bool Quad::IsOverlap(const glm::dvec2& currentPos, const Quad& quad)
{
	if (currentPos.x > quad.m_Position.x + quad.m_PositionOffset.x - (quad.m_Scale.x + quad.m_ScaleOffset.x) * 0.5f && currentPos.x < quad.m_Position.x + quad.m_PositionOffset.x + (quad.m_Scale.x + quad.m_ScaleOffset.x) * 0.5f &&
		currentPos.y > quad.m_Position.y + quad.m_PositionOffset.y - (quad.m_Scale.y + quad.m_ScaleOffset.y) * 0.5f && currentPos.y < quad.m_Position.y + quad.m_PositionOffset.y + (quad.m_Scale.y + quad.m_ScaleOffset.y) * 0.5f)
	{
		return true;
	}

	return false;
}

bool Quad::IsOverlapBorder(const glm::dvec2& currentPos, const Quad& quad, float borderWidth)
{
	bool leftSide = currentPos.x > quad.m_Position.x + quad.m_PositionOffset.x - (quad.m_Scale.x + quad.m_ScaleOffset.x) * 0.5f - borderWidth && currentPos.x < quad.m_Position.x + quad.m_PositionOffset.x - (quad.m_Scale.x + quad.m_ScaleOffset.x) * 0.5f;
	bool rightSide = currentPos.x > quad.m_Position.x + quad.m_PositionOffset.x + (quad.m_Scale.x + quad.m_ScaleOffset.x) * 0.5f && currentPos.x < quad.m_Position.x + quad.m_PositionOffset.x + (quad.m_Scale.x + quad.m_ScaleOffset.x) * 0.5f + borderWidth;
	bool topSide = currentPos.y > quad.m_Position.y + quad.m_PositionOffset.y - (quad.m_Scale.y + quad.m_ScaleOffset.y) * 0.5f - borderWidth && currentPos.y < quad.m_Position.y + quad.m_PositionOffset.y - (quad.m_Scale.y + quad.m_ScaleOffset.y) * 0.5f;
	bool bottomSide = currentPos.y > quad.m_Position.y + quad.m_PositionOffset.y + (quad.m_Scale.y + quad.m_ScaleOffset.y) * 0.5f && currentPos.y < quad.m_Position.y + quad.m_PositionOffset.y + (quad.m_Scale.y + quad.m_ScaleOffset.y) * 0.5f + borderWidth;

	if ((leftSide && topSide) || (leftSide && bottomSide) ||
		(rightSide && topSide) || (rightSide && bottomSide))
	{
		return true;
	}

	return false;
}

void Quad::SetPositionOffset(const glm::vec2& position)
{
	m_PositionOffset = position;
}

void Quad::SetScaleOffset(const glm::vec2& scale)
{
	m_ScaleOffset = scale;
}

void Quad::ClampScaleOffset(const glm::vec2& min, const glm::vec2& max)
{
	m_ScaleOffset = glm::clamp(m_ScaleOffset, min, max);
}

glm::vec2 Quad::m_PositionOffset;
glm::vec2 Quad::m_ScaleOffset;