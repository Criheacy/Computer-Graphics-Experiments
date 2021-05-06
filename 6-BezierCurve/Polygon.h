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
	Polygon(std::vector<glm::vec2> vertexes);
	Polygon(const Polygon& polygon);
	~Polygon();

	Polygon& operator= (const Polygon& polygon);

	void InitialTriangle(float a);
	void ClearVertex();

	Vertex* GetVertexHead() const;
	int GetVertexNumber() const;

	Vertex* CreateNewVertexOn(glm::vec2 position, Edge* attachedEdge);
	Vertex* CreateNewVertexAfter(glm::vec2 position, Vertex* preVertex);
	void RemoveVertex(Vertex* vertex, bool force = false);

	Edge* GetNearestEdge(glm::vec2 point);
	float GetDistanceToEdge(glm::vec2 point, Edge* edge);

	Vertex* GetNearestVertex(glm::vec2 point);
	float GetDistanceToVertex(glm::vec2 point, Vertex* vertex);

protected:
	Vertex* vertexHead;
	int vertexNumber;
};