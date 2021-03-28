#pragma once
#include "Polygon.h"

typedef int RD;	// Rect Direction

class Rect : public Polygon
{
public:
	Rect();
	void InitialRect(float centerX, float centerY, float width, float height);
	
	void MoveTo(RD dir, glm::vec2 dist);
	glm::vec2 GetDir(RD dir) const;

	void InitialTriangle(float a) = delete;
	void ClearVertex() = delete;
	Vertex* GetVertexHead() const = delete;
	int GetVertexNumber() const = delete;

	Vertex* CreateNewVertexOn(glm::vec2 position, Edge* attachedEdge) = delete;
	Vertex* CreateNewVertexAfter(glm::vec2 position, Vertex* preVertex) = delete;
	void RemoveVertex(Vertex* vertex, bool force = false) = delete;

	RD GetNearestEdge(glm::vec2 point);
	float GetDistanceToEdge(glm::vec2 point, RD edge);

	RD GetNearestVertex(glm::vec2 point);
	float GetDistanceToVertex(glm::vec2 point, RD vertex);

	constexpr static RD TOP    = 1 << 0;
	constexpr static RD LEFT   = 1 << 1;
	constexpr static RD BOTTOM = 1 << 2;
	constexpr static RD RIGHT  = 1 << 3;

private:
	float leftX;
	float topY;
	float rightX;
	float bottomY;
};