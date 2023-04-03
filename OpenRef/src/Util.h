#pragma once

#include "glm/glm.hpp"
#include "Renderer.h"

static bool IsOverlapSquare(const glm::dvec2& currentPos, const glm::vec2& targetPos, const glm::vec2& targetScale)
{
	if (currentPos.x > targetPos.x && currentPos.x < targetPos.x + targetScale.x &&
		currentPos.y > targetPos.y && currentPos.y < targetPos.y + targetScale.y)
	{
		return true;
	}

	return false;
}

static bool IsOverlapQuad(const glm::dvec2& currentPos, const Quad& quad)
{
	/*if (currentPos.x > quad.Position.x && currentPos.x < quad.Position.x + quad.Scale.x &&
		currentPos.y > quad.Position.y && currentPos.y < quad.Position.y + quad.Scale.y)
	{
		return true;
	}*/

	if (currentPos.x > quad.FinalPosition().x - quad.FinalScale().x * 0.5f && currentPos.x < quad.FinalPosition().x + quad.FinalScale().x * 0.5f &&
		currentPos.y > quad.FinalPosition().y - quad.FinalScale().y * 0.5f && currentPos.y < quad.FinalPosition().y + quad.FinalScale().y * 0.5f)
	{
		return true;
	}

	return false;
}

static bool IsOverlapQuadCorner(const glm::dvec2& currentPos, const Quad& quad, const glm::vec2& viewOffset, float borderWidth)
{
	/*bool leftSide = currentPos.x > quad.Position.x - borderWidth && currentPos.x < quad.Position.x;
	bool rightSide = currentPos.x > quad.Position.x + quad.Scale.x && currentPos.x < quad.Position.x + quad.Scale.x + borderWidth;
	bool topSide = currentPos.y > quad.Position.y - borderWidth && currentPos.y < quad.Position.y;
	bool bottomSide = currentPos.y > quad.Position.y + quad.Scale.y && currentPos.y < quad.Position.y + quad.Scale.y + borderWidth;*/

	bool leftSide = currentPos.x > quad.FinalPosition().x - quad.FinalScale().x * 0.5f - borderWidth && currentPos.x < quad.FinalPosition().x - quad.FinalScale().x * 0.5f;
	bool rightSide = currentPos.x > quad.FinalPosition().x + quad.FinalScale().x * 0.5f && currentPos.x < quad.FinalPosition().x + quad.FinalScale().x * 0.5f + borderWidth;
	bool topSide = currentPos.y > quad.FinalPosition().y - quad.FinalScale().y * 0.5f - borderWidth && currentPos.y < quad.FinalPosition().y - quad.FinalScale().y * 0.5f;
	bool bottomSide = currentPos.y > quad.FinalPosition().y + quad.FinalScale().y * 0.5f && currentPos.y < quad.FinalPosition().y + quad.FinalScale().y * 0.5f + borderWidth;

	if ((leftSide && topSide) || (leftSide && bottomSide) ||
		(rightSide && topSide) || (rightSide && bottomSide))
	{
		return true;
	}
	
	return false;
}