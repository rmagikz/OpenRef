#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

class Quad
{
private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
	GLfloat m_TexIndex;
	float m_AspectRatio;
	Texture* m_Texture;

	static glm::vec2 m_PositionOffset;
	static glm::vec2 m_ScaleOffset;
public:
	Quad();
	Quad(glm::vec2 position, GLfloat texIndex, const char* path);

	inline Texture* GetTexture() const { return m_Texture; }
	inline GLfloat TextureIndex() const { return m_TexIndex; }
	inline float AspectRatio() const { return m_AspectRatio; }
	inline glm::vec2 Position() const { return m_Position; }
	inline glm::vec2 Scale() const { return m_Scale; }
	inline glm::vec2 FinalPosition() const { return m_Position + m_PositionOffset; }
	inline glm::vec2 FinalScale() const { return m_Scale + glm::vec2(m_ScaleOffset.x, m_ScaleOffset.x / m_AspectRatio); }

	static inline glm::vec2 PositionOffset() { return m_PositionOffset; }
	static inline glm::vec2 ScaleOffset() { return m_ScaleOffset; }

	void Translate(const glm::vec2& translation);

	void SetPosition(const glm::vec2& position);

	void SetScale(const glm::vec2& scale);

	void CalculateAspectRatio();

	void ClampScale(const glm::vec2& min, const glm::vec2& max);

	static bool IsOverlap(const glm::dvec2& currentPos, const Quad& quad);

	static bool IsOverlapBorder(const glm::dvec2& currentPos, const Quad& quad, float borderWidth);

	static void SetPositionOffset(const glm::vec2& position);

	static void SetScaleOffset(const glm::vec2& scale);

	static void ClampScaleOffset(const glm::vec2& min, const glm::vec2& max);
};