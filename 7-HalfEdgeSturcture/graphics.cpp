#include "graphics.h"

Graphics::Graphics()
{
	graphicsIndex = Space::GetInstance().AttachGraphics(this);
	vertexCount = 0;
	edgeCount = 0;
	faceCount = 0;

	headVertex = nullptr;

	transform = glm::mat4(1.0f);
}

Graphics& Graphics::operator=(const Graphics& origin)
{
	// Skip unnecessary assignment
	if (this == &origin) return *this;

	Destroy();
	graphicsIndex = Space::GetInstance().AttachGraphics(this);

	// TODO: Implement deep copy

	return *this;
}

Graphics::Graphics(const Graphics& origin)
{
	*this = origin;
}

Graphics::~Graphics()
{
	Destroy();
}

/// Set vertices and edges of graphics. The vertices and indices data should passing through std::vector,
/// and then this function will use these data to create vertex and edge entities (in heap memory) and
/// form a Half-edge Structure. The structure will expose 'headVertex' pointer in Graphics class for access.
/// \param vertexArray Raw vertices data. Coordinate of vertices should be formatted as glm::vec3.
/// \param indexArray Raw indices data, each element represents a face. [NOTICE: Only supports triangular-faces
/// by now, that is faces represents by exact 3 indices]
void Graphics::SetGraphicsArray(const std::vector<glm::vec3>& vertexArray,
                                const std::vector<std::vector<int>>& indexArray)
{
	vertexCount = vertexArray.size();
	Vertex** vertexList = new Vertex*[vertexCount];
	for (int i = 0; i < vertexCount; i++)
	{
		vertexList[i] = new Vertex(i, vertexArray[i], nullptr);
	}

	// Hangs any vertex in graphics
	this->headVertex = vertexList[0];

	int faceCount = indexArray.size();
	for(int i = 0; i < faceCount; i++)
	{
		int edgeCountInFace = indexArray[i].size();
		Edge* firstEdge = nullptr;
		Edge* prevEdge = nullptr;
		for (int j = 0; j < edgeCountInFace; j++)
		{
			int edgeFrom = indexArray[i][j];
			// The index of to-vertex is the next of that of from-vertex in index array (in ring)
			int edgeTo = (j + 1 == edgeCountInFace) ? indexArray[i][0] : indexArray[i][j + 1];

			// Create new edge based on from and to vertex
			Edge* currentEdge = new Edge(vertexList[edgeFrom], vertexList[edgeTo], nullptr, nullptr);
			vertexList[edgeFrom]->AddEdge(currentEdge);

			// Check if opposite edge exists, link them if so
			Edge* oppositeEdge = vertexList[edgeTo]->GetEdgeTo(vertexList[edgeFrom]);
			if (oppositeEdge != nullptr)
			{
				currentEdge->opposite = oppositeEdge;
				oppositeEdge->opposite = currentEdge;
			}

			if (j == 0)
			{
				firstEdge = currentEdge;
			}
			// Store prevEdge for setting the '->next' for the next edge
			if (prevEdge != nullptr)
			{
				prevEdge->next = currentEdge;
			}
			prevEdge = currentEdge;
		}
		if (prevEdge != nullptr)
		{
			prevEdge->next = firstEdge;
		}
	}

	// deletes indexing list pointers, but DO NOT deletes vertex entities
	delete[] vertexList;
}

int Graphics::GetVertexCount() const
{
	return vertexCount;
}

GraphicsIterator Graphics::VertexBegin()
{
	return GraphicsIterator(headVertex);
}

GraphicsIterator Graphics::VertexEnd()
{
	return GraphicsIterator::end;
}

int Graphics::GetEdgeCount() const
{
	return edgeCount;
}

GraphicsIterator Graphics::EdgeBegin()
{
	return GraphicsIterator(headVertex->headEdge);
}

GraphicsIterator Graphics::EdgeEnd()
{
	return GraphicsIterator::end;
}

int Graphics::GetFaceCount() const
{
	return faceCount;
}

GraphicsIterator Graphics::FaceBegin()
{
	return GraphicsIterator(new Face(headVertex->headEdge));
}

GraphicsIterator Graphics::FaceEnd()
{
	return GraphicsIterator::end;
}

// Detach this graphics from space
// Clear all vertices and indices data
void Graphics::Destroy()
{
	if (graphicsIndex != nullptr)
		Space::GetInstance().DetachGraphics(graphicsIndex);
	// unnecessary for null pointer checking:
	// delete[] nullptr has no effect

	std::vector<Vertex*> verticesToDelete;
	std::vector<Edge*> edgesToDelete;
	for (GraphicsIterator it = VertexBegin(); it != VertexEnd(); ++it)
	{
		verticesToDelete.push_back(dynamic_cast<Vertex*>(*it));
	}
	for (GraphicsIterator it = EdgeBegin(); it != EdgeEnd(); ++it)
	{
		edgesToDelete.push_back(dynamic_cast<Edge*>(*it));
	}

	// TODO: Implement vertices and edges deletion
}
