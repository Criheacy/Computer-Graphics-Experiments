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

Graphics& Graphics::operator=(const Graphics& origin)
{
	// Skip unnecessary assignment
	if (this == &origin) return *this;

	Destroy();
	graphicsIndex = Space::GetInstance().AttachGraphics(this);

	vertexArraySize = origin.vertexArraySize;
	vertexArray = new glm::vec3[vertexArraySize];
	for (int i=0; i<vertexArraySize; i++)
	{
		vertexArray[i] = origin.vertexArray[i];
	}
	vertexIndicesSize = origin.vertexIndicesSize;
	vertexIndices = new glm::vec3[vertexIndicesSize];
	for (int i=0; i<vertexIndicesSize; i++)
	{
		vertexIndices[i] = origin.vertexIndices[i];
	}
	return *this;
}

Graphics::Graphics(const Graphics& origin)
{
	graphicsIndex = Space::GetInstance().AttachGraphics(this);

	vertexArraySize = origin.vertexArraySize;
	vertexArray = new glm::vec3[vertexArraySize];
	for (int i=0; i<vertexArraySize; i++)
	{
		vertexArray[i] = origin.vertexArray[i];
	}
	vertexIndicesSize = origin.vertexIndicesSize;
	vertexIndices = new glm::vec3[vertexIndicesSize];
	for (int i=0; i<vertexIndicesSize; i++)
	{
		vertexIndices[i] = origin.vertexIndices[i];
	}
	transform = origin.transform;
}

Graphics::~Graphics()
{
	Destroy();
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

// Detach this graphics from space
// Clear all vertices and indices data
void Graphics::Destroy()
{
	if (graphicsIndex != nullptr)
		Space::GetInstance().DetachGraphics(graphicsIndex);
	// unnecessary for null pointer checking:
	// delete[] nullptr has no effect

	// FIXME: Destroy function raises exception when pointers 'vertexIndices' or 'vertexArray' are illegal
	delete[] vertexIndices;
	delete[] vertexArray;
	vertexIndices = nullptr;
	vertexArray = nullptr;
}

// Debug log test
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
