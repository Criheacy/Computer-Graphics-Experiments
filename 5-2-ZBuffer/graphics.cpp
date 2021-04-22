#include "graphics.h"

Graphics::Graphics()
{
	graphicsIndex = Space::GetInstance().AttachGraphics(this);
	vertexIndicesSize = 0;
	vertexArraySize = 0;
	vertexIndices = nullptr;
	vertexArray = nullptr;

	transform = glm::mat4();
}

Graphics::~Graphics()
{
	Space::GetInstance().DetachGraphics(graphicsIndex);
}

unsigned int Graphics::GetVertexArraySize() const
{
	return vertexArraySize;
}

glm::vec3* Graphics::GetVertexArrayPtr()
{
	return vertexArray;
}

unsigned int Graphics::GetVertexIndicesSize() const
{
	return vertexIndicesSize;
}

glm::vec3* Graphics::GetVertexIndicesPtr()
{
	return vertexIndices;
}

void Graphics::LogTest()
{
	printf("========== Graphics Info ==========\n");
	printf(" -> vertex: %d\n", vertexArraySize);
	printf(" -> vertex list:\n");
	for (int i = 0; i < vertexArraySize; i++)
	{
		printf("\t(%.1f, %.1f, %.1f)\n", vertexArray[i].x, vertexArray[i].y, vertexArray[i].z);
	}

	printf(" -> index: %d\n", vertexIndicesSize);
	for (int i = 0; i < vertexIndicesSize; i++)
	{
		printf("\t[%d -> %d -> %d]\n", (int)vertexIndices[i].x, (int)vertexIndices[i].y, (int)vertexIndices[i].z);
	}
	printf("===================================\n\n");
}
