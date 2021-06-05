#pragma once
#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <glm/vec3.hpp>
#include <map>
#include <queue>
#include <vector>

class GraphicsComponent {
public:
	GraphicsComponent() = default;
	virtual ~GraphicsComponent() = 0;
	virtual std::vector<GraphicsComponent *> GetAdjacentComponent() = 0;
	virtual bool operator<(const GraphicsComponent &rhs) const = 0;

	bool operator<=(const GraphicsComponent &rhs) const;
	bool operator>(const GraphicsComponent &rhs) const;
	bool operator>=(const GraphicsComponent &rhs) const;
	bool operator==(const GraphicsComponent &rhs) const;
	bool operator!=(const GraphicsComponent &rhs) const;
};

struct MapComponentComparer {
	bool operator()(const GraphicsComponent *a, const GraphicsComponent *b);
};

class Vertex;
class Edge;
class Face;

class Vertex : public GraphicsComponent {
public:
	Vertex() = default;
	Vertex(int index, glm::vec3 position, Edge *headEdge);
	~Vertex() override;

	void AddEdge(Edge *newEdge);
	void RemoveEdge(Edge *edgeToRemove);
	Edge *GetEdgeTo(Vertex *toVertex) const;

	bool operator<(const GraphicsComponent &rhs) const override;
	std::vector<GraphicsComponent *> GetAdjacentComponent() override;

	int index;
	glm::vec3 position;
	Edge *headEdge;
};

class Edge : public GraphicsComponent {
public:
	Edge() = default;
	Edge(Vertex *from, Vertex *to, Edge *opposite = nullptr, Edge *next = nullptr, Edge *follow = nullptr);
	~Edge() override;

	bool operator<(const GraphicsComponent &rhs) const override;
	std::vector<GraphicsComponent *> GetAdjacentComponent() override;

	Vertex *from;
	Vertex *to;
	Edge *opposite;
	Edge *next;
	Edge *follow;
};

class Face : public GraphicsComponent {
public:
	Face() = default;
	explicit Face(Edge *markedEdge);
	~Face() override;

	bool operator<(const GraphicsComponent &rhs) const override;
	std::vector<GraphicsComponent *> GetAdjacentComponent() override;

	Edge *markedEdge;

private:
	void MinimizeMarkedEdge();
};

class GraphicsIterator {
public:
	GraphicsIterator(const GraphicsIterator &rhs);
	GraphicsIterator &operator=(const GraphicsIterator &rhs);
	bool operator==(const GraphicsIterator &rhs) const;
	bool operator!=(const GraphicsIterator &rhs) const;
	GraphicsComponent *operator*() const;
	GraphicsIterator &operator++();// ++i

	// [Deprecated] virtual GraphicsIterator& operator++(int);     // i++
	// since operator++ of this iterator always returns itself, back-side
	// (i++) should returns a copy of itself, which is neither convenient
	// nor efficient, so please use front-side (++i) instead.

	static const GraphicsIterator end;

private:
	friend class Graphics;
	explicit GraphicsIterator(GraphicsComponent *beginComponent);
	void UpdateComponent(GraphicsComponent *component);
	std::map<GraphicsComponent *, bool, MapComponentComparer> inQueueMap;
	std::queue<GraphicsComponent *> nextQueue;
};

#endif//GRAPHICS_COMPONENT_H
