#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_component.h"
#include "space.h"
#include <cstdio>
#include <glm/mat4x4.hpp>

class Graphics {
public:
	Graphics();

	Graphics(Graphics const &origin);
	Graphics &operator=(Graphics const &origin);

	~Graphics();

	void Destroy();

	void SetGraphicsArray(const std::vector<glm::vec3> &vertexArray,
	                      const std::vector<std::vector<int>> &indexArray);

	// Modify
	void InsertVertexInEdge(Edge *insertedEdge);
	void InsertVertexInEdge(Edge *insertedEdge, glm::vec3 vertexPosition);
	void SubdivideFaces();

	int GetVertexCount() const;
	GraphicsIterator VertexBegin();
	static GraphicsIterator VertexEnd();

	int GetEdgeCount() const;
	GraphicsIterator EdgeBegin();
	static GraphicsIterator EdgeEnd();

	int GetFaceCount() const;
	GraphicsIterator FaceBegin();
	static GraphicsIterator FaceEnd();

protected:
	// The following functions may destroy the half-side structure of the graph,
	// and need to be called indirectly through the open interfaces

	/// Insert the vertex into the given edge. This function do NOTHING to opposite side,
	/// and will increase the face indices (triangular-faces will be changed to quads)
	static void InsertVertexInEdgeRaw(Vertex *vertexToInsert, Edge *insertedEdge);

	/// Divide Face into 2 sub-faces. Generate 2 new edge by division line.
	static void DivideFace(Face* faceToDivide, Vertex *firstVertex, Vertex *secondVertex);

	glm::mat4x4 transform;
	struct GraphicsLinkNode *graphicsIndex;

	int vertexCount;
	int edgeCount;
	int faceCount;

private:
	Vertex *headVertex;
};

#endif//GRAPHICS_H
