#pragma once
#include "Settings.h"

class Polygon
{
public:
	struct Vertex
	{
		glm::vec2 vertex;
		Vertex* next;
	};

	struct Edge
	{
		Vertex* from;
		Vertex* to;
	};

public:
	Polygon();
	void InitialTriangle(float a);
	void ClearVertex();

	Vertex* GetVertexHead();
	int GetVertexNumber();

	Vertex* CreateNewVertexOn(glm::vec2 position, Edge* attachedEdge);
	void RemoveVertex(Vertex* vertex, bool force = false);

	Edge* GetNearestEdge(glm::vec2 point);
	float GetDistanceToEdge(glm::vec2 point, Edge* edge);

	Vertex* GetNearestVertex(glm::vec2 point);
	float GetDistanceToVertex(glm::vec2 point, Vertex* vertex);

private:
	Vertex* vertexHead;
	int vertexNumber;
};