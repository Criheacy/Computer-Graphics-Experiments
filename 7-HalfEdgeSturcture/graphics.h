#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "space.h"
#include "graphics_component.h"
#include <cstdio>
#include <glm/mat4x4.hpp>

class Graphics
{
public:
	Graphics();

	Graphics(Graphics const& origin);
	Graphics& operator=(Graphics const& origin);

	~Graphics();

	void Destroy();

	void SetGraphicsArray(const std::vector<glm::vec3>& vertexArray,
	                      const std::vector<std::vector<int>>& indexArray);

	int GetVertexCount() const;
	GraphicsIterator VertexBegin();
	static GraphicsIterator VertexEnd();

	int GetEdgeCount() const;
	GraphicsIterator EdgeBegin();
	static GraphicsIterator EdgeEnd();

	int GetFaceCount() const;
	GraphicsIterator FaceBegin();
	static GraphicsIterator FaceEnd();

	void LogTest();

protected:
	glm::mat4x4 transform;
	struct GraphicsLinkNode* graphicsIndex;

	int vertexCount;
	int edgeCount;
	int faceCount;

private:
	Vertex* headVertex;
};

#endif  //GRAPHICS_H
