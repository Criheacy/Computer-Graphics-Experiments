#pragma once
#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <glm/vec3.hpp>
#include <vector>
#include <map>
#include <queue>

struct GraphicsComponent
{
	virtual std::vector<GraphicsComponent*> GetAdjacentComponent() = 0;
	virtual bool operator<(const GraphicsComponent& rhs) const = 0;

	bool operator<=(const GraphicsComponent& rhs) const;
	bool operator>(const GraphicsComponent& rhs) const;
	bool operator>=(const GraphicsComponent& rhs) const;
	bool operator==(const GraphicsComponent& rhs) const;
	bool operator!=(const GraphicsComponent& rhs) const;
};

struct MapComponentComparer
{
	bool operator()(const GraphicsComponent* a, const GraphicsComponent* b);
};

struct Vertex;
struct Edge;
struct Face;

struct Vertex : public GraphicsComponent
{
	Vertex() = default;
	Vertex(int index, glm::vec3 position, Edge* headEdge);

	int index;
	glm::vec3 position;
	Edge* headEdge;

	void AddEdge(Edge* newEdge);
	void RemoveEdge(Edge* edgeToRemove);

	Edge* GetEdgeTo(Vertex* toVertex) const;

	bool operator<(const GraphicsComponent& rhs) const override;
	std::vector<GraphicsComponent*> GetAdjacentComponent() override;
};

struct Edge : public GraphicsComponent
{
	Edge() = default;
	Edge(Vertex* from, Vertex* to, Edge* opposite = nullptr, Edge* next = nullptr, Edge* follow = nullptr);
	Vertex* from;
	Vertex* to;
	Edge* opposite;
	Edge* next;
	Edge* follow;
	bool operator<(const GraphicsComponent& rhs) const override;
	std::vector<GraphicsComponent*> GetAdjacentComponent() override;
};

struct Face : public GraphicsComponent
{
	Face() = default;
	explicit Face(Edge* markedEdge);
	Edge* markedEdge;
	bool operator<(const GraphicsComponent& rhs) const override;
	std::vector<GraphicsComponent*> GetAdjacentComponent() override;
};

class GraphicsIterator
{
public:
	GraphicsIterator(const GraphicsIterator& rhs);
	GraphicsIterator& operator=(const GraphicsIterator& rhs);
	bool operator==(const GraphicsIterator& rhs) const;
	bool operator!=(const GraphicsIterator& rhs) const;
	GraphicsComponent* operator*() const;
	GraphicsIterator& operator++();     // ++i

	// [Deprecated] virtual GraphicsIterator& operator++(int);     // i++
	// since operator++ of this iterator always returns itself, back-side
	// (i++) should returns a copy of itself, which is neither convenient
	// nor efficient, so please use front-side (++i) instead.

	static const GraphicsIterator end;

private:
	friend class Graphics;
	explicit GraphicsIterator(GraphicsComponent* beginComponent);
	void UpdateComponent(GraphicsComponent* component);
	std::map<GraphicsComponent*, bool, MapComponentComparer> inQueueMap;
	std::queue<GraphicsComponent*> nextQueue;
};

#endif  //GRAPHICS_COMPONENT_H
