#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "space.h"
#include <cstdio>
#include <glm/mat4x4.hpp>
class Graphics
{
public:
	Graphics();
	~Graphics();

	unsigned int GetVertexArraySize() const;
	glm::vec3* GetVertexArrayPtr();
	unsigned int GetVertexIndicesSize() const;
	glm::vec3* GetVertexIndicesPtr();

	void LogTest();

protected:
	glm::vec3* vertexArray;
	int vertexArraySize;

	glm::vec3* vertexIndices;
	int vertexIndicesSize;

	glm::mat4x4 transform;

	struct GraphicsLinkNode* graphicsIndex;
};

#endif  //GRAPHICS_H
