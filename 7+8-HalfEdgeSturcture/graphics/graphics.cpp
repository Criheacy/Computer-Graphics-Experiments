#include "graphics.h"
#include "../utils/progress_bar.h"

Graphics::Graphics() {
	graphicsIndex = Space::GetInstance().AttachGraphics(this);
	vertexCount = 0;
	edgeCount = 0;
	faceCount = 0;

	headVertex = nullptr;

	transform = glm::mat4(1.0f);
}

Graphics &Graphics::operator=(const Graphics &origin) {
	// Skip unnecessary assignment
	if (this == &origin) return *this;

	Destroy();
	graphicsIndex = Space::GetInstance().AttachGraphics(this);

	// TODO: Implement deep copy

	return *this;
}

Graphics::Graphics(const Graphics &origin) {
	*this = origin;
}

Graphics::~Graphics() {
	Destroy();
}

/// Set vertices and edges of graphics. The vertices and indices data should passing through std::vector,
/// and then this function will use these data to create vertex and edge entities (in heap memory) and
/// form a Half-edge Structure. The structure will expose 'headVertex' pointer in Graphics class for access.
/// \param vertexArray Raw vertices data. Coordinate of vertices should be formatted as glm::vec3.
/// \param indexArray Raw indices data, each element represents a face. [NOTICE: Only supports triangular-faces
/// by now, that is faces represents by exact 3 indices]
void Graphics::SetGraphicsArray(const std::vector<glm::vec3> &vertexArray,
                                const std::vector<std::vector<int>> &indexArray) {
	vertexCount = vertexArray.size();
	ProgressBar vertexProgressBar(vertexCount);
	vertexProgressBar.SetProgressTitle("Setting graphics array [1/2] - converting vertices");
	vertexProgressBar.SetInterceptInterval(1);

	Vertex **vertexList = new Vertex *[vertexCount];
	for (int i = 0; i < vertexCount; i++) {
		vertexList[i] = new Vertex(i, vertexArray[i], nullptr);
		vertexProgressBar.SetProgressValue(i + 1);
		vertexProgressBar.Log();
	}

	// Hangs any vertex in graphics
	this->headVertex = vertexList[0];

	faceCount = indexArray.size();
	ProgressBar indexProgressBar(faceCount);
	indexProgressBar.SetProgressTitle("Setting graphics array [2/2] - converting indices");
	indexProgressBar.SetInterceptInterval(1);
	for (int i = 0; i < faceCount; i++) {
		int edgeCountInFace = indexArray[i].size();
		Edge *firstEdge = nullptr;
		Edge *prevEdge = nullptr;
		for (int j = 0; j < edgeCountInFace; j++) {
			int edgeFrom = indexArray[i][j];
			// The index of to-vertex is the next of that of from-vertex in index array (in ring)
			int edgeTo = (j + 1 == edgeCountInFace) ? indexArray[i][0] : indexArray[i][j + 1];

			// Create new edge based on from and to vertex
			Edge *currentEdge = new Edge(vertexList[edgeFrom], vertexList[edgeTo], nullptr, nullptr);
			vertexList[edgeFrom]->AddEdge(currentEdge);

			// Check if opposite edge exists, link them if so
			Edge *oppositeEdge = vertexList[edgeTo]->GetEdgeTo(vertexList[edgeFrom]);
			if (oppositeEdge != nullptr) {
				currentEdge->opposite = oppositeEdge;
				oppositeEdge->opposite = currentEdge;
			}

			if (j == 0) {
				firstEdge = currentEdge;
			}
			// Store prevEdge for setting the '->next' for the next edge
			if (prevEdge != nullptr) {
				prevEdge->next = currentEdge;
			}
			prevEdge = currentEdge;
		}
		if (prevEdge != nullptr) {
			prevEdge->next = firstEdge;
		}

		indexProgressBar.SetProgressValue(i + 1);
		indexProgressBar.Log();
	}

	// deletes indexing list pointers, but DO NOT deletes vertex entities
	delete[] vertexList;
}

void Graphics::InsertVertexInEdge(Edge *insertedEdge) {
	glm::vec3 middlePosition = (insertedEdge->from->position + insertedEdge->to->position) / 2.0f;
	InsertVertexInEdge(insertedEdge, middlePosition);
}

void Graphics::InsertVertexInEdge(Edge *insertedEdge, glm::vec3 vertexPosition) {
	Vertex* vertexToInsert = new Vertex(GetVertexCount(), vertexPosition, nullptr);
	++vertexCount;

	Edge* oppositeEdge = insertedEdge->opposite;
	InsertVertexInEdgeRaw(vertexToInsert, insertedEdge);
	InsertVertexInEdgeRaw(vertexToInsert, insertedEdge->opposite);

	insertedEdge->opposite = oppositeEdge->next;
	oppositeEdge->next->opposite = insertedEdge;
	insertedEdge->next->opposite = oppositeEdge;
	oppositeEdge->opposite = insertedEdge->next;
	edgeCount += 3;

	Face* faceToDivide;
	faceToDivide = new Face(insertedEdge);
	DivideFace(faceToDivide, insertedEdge->to, insertedEdge->next->next->to);
	delete faceToDivide;
	++faceCount;

	faceToDivide = new Face(insertedEdge->opposite);
	DivideFace(faceToDivide, insertedEdge->to, insertedEdge->opposite->next->to);
	delete faceToDivide;
	++faceCount;
}

void Graphics::SubdivideFaces() {
	// Subdivide all edges in middle point
	std::vector<Edge*> edgeList;
	for (auto it = EdgeBegin(); it != EdgeEnd(); ++it) {
		edgeList.push_back(dynamic_cast<Edge*>(*it));
	}

	ProgressBar edgeProgressBar(static_cast<int>(edgeList.size()));
	edgeProgressBar.SetProgressTitle("Subdivide faces [1/2] - inserting vertices");
	edgeProgressBar.SetInterceptInterval(100);

	for (auto it = edgeList.begin(); it != edgeList.end(); ++it) {
		// Check if the opposite edge has inserted a vertex
		if ((*it)->opposite->opposite == (*it)) {
			// Create a new vertex and insert it
			glm::vec3 middlePosition = ((*it)->from->position + (*it)->to->position) / 2.0f;
			Vertex* vertexToInsert = new Vertex(GetVertexCount(), middlePosition, nullptr);
			++vertexCount;
			++edgeCount;
			InsertVertexInEdgeRaw(vertexToInsert, (*it));
			// Mark this edge
			(*it)->opposite = nullptr;
		} else {
			// Get vertex inserted by opposite and insert to the edge itself
			Vertex* vertexToInsert = (*it)->opposite->to;
			InsertVertexInEdgeRaw(vertexToInsert, (*it));
			// Update opposite edge
			(*it)->opposite->opposite = (*it)->next;
			(*it)->next->opposite = (*it)->opposite;
			(*it)->opposite->next->opposite = (*it);
			(*it)->opposite = (*it)->opposite->next;
		}
		edgeProgressBar.UpdateProgressValue(1);
		edgeProgressBar.Log();
	}

	// Connect the inserted vertices to divide the faces
	std::vector<Face*> faceList;

	for (auto it = FaceBegin(); it != FaceEnd(); ++it) {
		faceList.push_back(dynamic_cast<Face *>(*it));
	}

	ProgressBar faceProgressBar(static_cast<int>(faceList.size()));
	faceProgressBar.SetProgressTitle("Subdivide faces [2/2] - divide faces");
	faceProgressBar.SetInterceptInterval(100);

	for (auto it = faceList.begin(); it != faceList.end(); ++it) {
		// Vertices just created always has larger indices than original vertices,
		// and the index of from vertex of marked edge of each faces is always the
		// smallest in the entire face, so the from vertex of marked edge of each
		// faces is always the original vertex (is NOT just created)
		Edge* currentEdge = dynamic_cast<Face*>(*it)->markedEdge;
		// The to vertex of marked edge (MUST be just created)
		Vertex* firstCreatedVertex = currentEdge->to;
		do {
			Face* faceToDivide = new Face(currentEdge);
			DivideFace(faceToDivide, currentEdge->to, currentEdge->next->next->to);
			++faceCount;
			++edgeCount;
			delete faceToDivide;
			currentEdge = currentEdge->next;
		} while (currentEdge->to != firstCreatedVertex);

		faceProgressBar.UpdateProgressValue(1);
		faceProgressBar.Log();
	}
}

int Graphics::GetVertexCount() const {
	return vertexCount;
}

GraphicsIterator Graphics::VertexBegin() {
	return GraphicsIterator(headVertex);
}

GraphicsIterator Graphics::VertexEnd() {
	return GraphicsIterator::end;
}

int Graphics::GetEdgeCount() const {=
	return edgeCount;
}

GraphicsIterator Graphics::EdgeBegin() {
	return GraphicsIterator(headVertex->headEdge);
}

GraphicsIterator Graphics::EdgeEnd() {
	return GraphicsIterator::end;
}

int Graphics::GetFaceCount() const {
	return faceCount;
}

GraphicsIterator Graphics::FaceBegin() {
	return GraphicsIterator(new Face(headVertex->headEdge));
}

GraphicsIterator Graphics::FaceEnd() {
	return GraphicsIterator::end;
}

// Detach this graphics from space
// Clear all vertices and indices data
void Graphics::Destroy() {
	if (graphicsIndex != nullptr)
		Space::GetInstance().DetachGraphics(graphicsIndex);
	// unnecessary for null pointer checking:
	// delete[] nullptr has no effect

	std::vector<Vertex *> verticesToDelete;
	std::vector<Edge *> edgesToDelete;
	for (GraphicsIterator it = VertexBegin(); it != VertexEnd(); ++it) {
		verticesToDelete.push_back(dynamic_cast<Vertex *>(*it));
	}
	for (GraphicsIterator it = EdgeBegin(); it != EdgeEnd(); ++it) {
		edgesToDelete.push_back(dynamic_cast<Edge *>(*it));
	}

	// TODO: Implement vertices and edges deletion
}

void Graphics::InsertVertexInEdgeRaw(Vertex *vertexToInsert, Edge *insertedEdge) {
	Edge *newNextEdge = new Edge(vertexToInsert, insertedEdge->to);
	insertedEdge->to = vertexToInsert;

	newNextEdge->next = insertedEdge->next;
	insertedEdge->next = newNextEdge;

	vertexToInsert->AddEdge(newNextEdge);
}

void Graphics::DivideFace(Face* faceToDivide, Vertex *firstVertex, Vertex *secondVertex) {
	Edge* firstToSecond = new Edge(firstVertex, secondVertex);
	Edge* secondToFirst = new Edge(secondVertex, firstVertex);

	firstToSecond->opposite = secondToFirst;
	secondToFirst->opposite = firstToSecond;

	firstVertex->AddEdge(firstToSecond);
	secondVertex->AddEdge(secondToFirst);

	// Update next edge link
	Edge* currentEdge = faceToDivide->markedEdge;
	do {
		Edge* nextEdge = currentEdge->next;
		if (currentEdge->to == firstVertex) {
			secondToFirst->next = currentEdge->next;
			currentEdge->next = firstToSecond;
		}
		if (currentEdge->to == secondVertex) {
			firstToSecond->next = currentEdge->next;
			currentEdge->next = secondToFirst;
		}
		currentEdge = nextEdge;
	} while (currentEdge != faceToDivide->markedEdge);
}
