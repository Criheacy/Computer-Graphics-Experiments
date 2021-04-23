#include "space.h"

Space &Space::GetInstance()
{
	static Space instance;
	return instance;
}

Space::Space()
{
	head = nullptr;
	graphicsUpdateFlag = true;
	serializedVerticesArraySize = 0;
	serializedVerticesArray = nullptr;
}

GraphicsLinkNode* Space::AttachGraphics(Graphics *graphics)
{
	// Graphics list has changed
	graphicsUpdateFlag = true;

	if (head == nullptr)
	{
		head = new GraphicsLinkNode;
		head->graphics = graphics;
		head->next = head;
		head->pre = head;
		return head;
	}
	else
	{
		GraphicsLinkNode* newNode = new GraphicsLinkNode;
		newNode->graphics = graphics;
		newNode->next = head->next;
		newNode->pre = head;
		head->next->pre = newNode;
		head->next = newNode;
		return newNode;
	}
}

void Space::DetachGraphics(GraphicsLinkNode *graphicsIndex)
{
	graphicsIndex->next->pre = graphicsIndex->pre;
	graphicsIndex->pre->next = graphicsIndex->next;
	if (graphicsIndex == head) head = nullptr;
	delete graphicsIndex;

	// Graphics list has changed
	graphicsUpdateFlag = true;
}

// Update serialized-vertices-array and serialized-indices-array
// Called when vertices data are required, after graphics list is changed
void Space::UpdateGraphicsVerticesArray()
{
	if (head == nullptr)
	{
		serializedVerticesArraySize = 0;
		serializedVerticesArray = nullptr;
		return;
	}
	delete[] serializedVerticesArray;
	delete[] serializedIndicesArray;

	serializedVerticesArraySize = 0;
	serializedIndicesArray = 0;
	GraphicsLinkNode* nowNode;

	// Accumulate number of vertices in all attached graphics
	nowNode = head;
	do {
		serializedVerticesArraySize
		    += nowNode->graphics->GetVertexArraySize();
		serializedIndicesArraySize
		    += nowNode->graphics->GetVertexIndicesSize();
		nowNode = nowNode->next;
	} while (nowNode != head);

	serializedVerticesArraySize *= 3;
	serializedIndicesArraySize *= 3;
	// Reallocate space for new arrays
	serializedVerticesArray = new float[serializedVerticesArraySize];
	serializedIndicesArray = new int[serializedIndicesArraySize];

	// Enumerate and add vertex info to serialized vertices array
	nowNode = head;
	int vertexCnt = 0;
	int indexCnt = 0;
	do {
		unsigned int graphicsVerticesNumber
		    = nowNode->graphics->GetVertexArraySize();
		glm::vec3* graphicsVertices
		    = nowNode->graphics->GetVertexArrayPtr();

		unsigned int graphicsIndicesNumber
		    = nowNode->graphics->GetVertexIndicesSize();
		glm::vec3* graphicsIndices
		    = nowNode->graphics->GetVertexIndicesPtr();

		// Serialize indices first
		for (unsigned int i = 0; i < graphicsIndicesNumber; i++)
		{
			// Indices number needs to add vertex-cnt to match origin vertex in space group
			serializedIndicesArray[indexCnt * 3] = (int)graphicsIndices[i].x + vertexCnt;
			serializedIndicesArray[indexCnt * 3 + 1] = (int)graphicsIndices[i].y + vertexCnt;
			serializedIndicesArray[indexCnt * 3 + 2] = (int)graphicsIndices[i].z + vertexCnt;
			indexCnt++;
		}

		for (unsigned int i = 0; i < graphicsVerticesNumber; i++)
		{
			// Serialize vector data into 3 floats
			serializedVerticesArray[vertexCnt * 3] = graphicsVertices[i].x;
			serializedVerticesArray[vertexCnt * 3 + 1] = graphicsVertices[i].y;
			serializedVerticesArray[vertexCnt * 3 + 2] = graphicsVertices[i].z;
			vertexCnt++;
		}
		nowNode = nowNode->next;
	} while (nowNode != head);

	// Update complete, clear state flag
	graphicsUpdateFlag = false;
}

unsigned int Space::GetSerializedVerticesArraySize()
{
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedVerticesArraySize;
}

float* Space::GetSerializedVerticesArrayPtr()
{
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedVerticesArray;
}

unsigned int Space::GetSerializedIndicesArraySize()
{
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedIndicesArraySize;
}

int* Space::GetSerializedIndicesArrayPtr()
{
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();
	return serializedIndicesArray;
}

void Space::LogTest()
{
	if (graphicsUpdateFlag) UpdateGraphicsVerticesArray();

	printf("========== Space Info ==========\n");
	printf(" -> vertex: %d\n", serializedVerticesArraySize);
	printf(" -> vertex list:\n");
	for (unsigned int i = 0; i < serializedVerticesArraySize; i += 3)
	{
		printf("\t%.1f, %.1f, %.1f,\n",
		       *(serializedVerticesArray + i),
		       *(serializedVerticesArray + i + 1),
		       *(serializedVerticesArray + i + 2));
	}

	printf(" -> index: %d\n", serializedIndicesArraySize);
	for (unsigned int i = 0; i < serializedIndicesArraySize; i += 3)
	{
		printf("\t%d, %d, %d\n",
		       *(serializedIndicesArray + i),
		       *(serializedIndicesArray + i + 1),
		       *(serializedIndicesArray + i + 2));
	}
	printf("===================================\n\n");
}
