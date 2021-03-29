#include "Polygon.h"

Polygon::Polygon()
{
	InitialTriangle(300);
}

Polygon::Polygon(std::vector<glm::vec2> vertexes)
{
	vertexNumber = vertexes.size();
	Vertex* nowVertex = new Vertex{ vertexes[0], nullptr };
	vertexHead = nowVertex;

	for (int i = 1; i < vertexNumber; i++)
	{
		nowVertex->next = new Vertex{ vertexes[i], nullptr };
		nowVertex = nowVertex->next;
	}
	nowVertex->next = vertexHead;
}

Polygon::Polygon(const Polygon& polygon)
{
	this->ClearVertex();
	this->vertexNumber = polygon.GetVertexNumber();

	this->vertexHead = new Vertex{ polygon.GetVertexHead()->vertex, nullptr };
	Vertex* preVertex = this->vertexHead;
	Vertex* vertex = polygon.GetVertexHead()->next;
	while (vertex != polygon.GetVertexHead())
	{
		Vertex* newVertex = new Vertex{ vertex->vertex, nullptr };
		preVertex->next = newVertex;
		preVertex = newVertex;
		vertex = vertex->next;
	};
	preVertex->next = this->vertexHead;
}

Polygon::~Polygon()
{
	ClearVertex();
}

Polygon& Polygon::operator=(const Polygon& polygon)
{
	Polygon* newPolygon = new Polygon(polygon);
	return *newPolygon;
}

void Polygon::InitialTriangle(float a)
{
	ClearVertex();
	float sq3 = sqrtf(3);
	Vertex* top = new Vertex{ glm::vec2((int)roundf(a / 2), (int)roundf(-sq3 * a / 6)), nullptr };
	Vertex* left = new Vertex{ glm::vec2((int)roundf(-a / 2), (int)roundf(-sq3 * a / 6)), nullptr };
	Vertex* right = new Vertex{ glm::vec2(0, (int)roundf(sq3 * a / 3)), nullptr };
	top->next = left;
	left->next = right;
	right->next = top;

	vertexHead = top;
	vertexNumber = 3;
}

void Polygon::ClearVertex()
{
	vertexNumber = 0;
	if (vertexHead == nullptr) return;
	Vertex* nextVertex;
	while (vertexHead->next != vertexHead)
	{
		nextVertex = vertexHead->next->next;
		delete vertexHead->next;
		vertexHead->next = nextVertex;
	}
	delete vertexHead;
	vertexHead = nullptr;
}

Polygon::Vertex* Polygon::GetVertexHead() const
{
	return vertexHead;
}

int Polygon::GetVertexNumber() const
{
	return vertexNumber;
}

Polygon::Vertex* Polygon::CreateNewVertexOn(glm::vec2 position, Edge* attachedEdge)
{
	Vertex* fromVertex = attachedEdge->from;
	Vertex* toVertex = attachedEdge->to;
	if (fromVertex->next != attachedEdge->to)
		std::swap(fromVertex, toVertex);
	if (fromVertex->next != attachedEdge->to)
	{
		printf("[LOG] In 'CreateNewVertexOn': Invalid Edge\n");
		return nullptr;
	}

	Vertex* newVertex = new Vertex{ position, fromVertex->next };
	fromVertex->next = newVertex;
	vertexNumber++;
	return newVertex;
}

Polygon::Vertex* Polygon::CreateNewVertexAfter(glm::vec2 position, Vertex* preVertex)
{
	Vertex* newVertex = new Vertex{ position, preVertex->next };
	preVertex->next = newVertex;
	vertexNumber++;
	return newVertex;
}

void Polygon::RemoveVertex(Vertex* vertex, bool force)
{
	if (GetVertexNumber() == 0) return;
	if ((GetVertexNumber() <= 3) && !force) return;
	
	Vertex* preVertex = vertexHead;
	Vertex* nowVertex = vertexHead->next;
	do
	{
		if (nowVertex == vertex)
		{
			preVertex->next = nowVertex->next;
			if (nowVertex == vertexHead)
			{
				if (vertexHead->next == vertexHead)
					vertexHead = nullptr;
				else vertexHead = preVertex;
			}
			delete nowVertex;
			nowVertex = nullptr;
			vertexNumber--;
			break;
		}
		preVertex = nowVertex;
		nowVertex = nowVertex->next;
	} while (nowVertex != vertexHead->next);
}

Polygon::Edge* Polygon::GetNearestEdge(glm::vec2 point)
{
	float minDistance = (1 << 30);
	Edge* minEdge = new Edge{ nullptr, nullptr };
	Edge* nowEdge = new Edge{ nullptr, nullptr };

	if (GetVertexNumber() < 2)
		return nullptr;

	Vertex* nowVertex = vertexHead;
	do
	{
		nowEdge->from = nowVertex;
		nowEdge->to = nowVertex->next;
		float dist = GetDistanceToEdge(point, nowEdge);
		if (dist < minDistance)
		{
			minDistance = dist;
			minEdge->from = nowEdge->from;
			minEdge->to = nowEdge->to;
		}
		nowVertex = nowVertex->next;
	} while (nowVertex != vertexHead);
	delete nowEdge;
	nowEdge = nullptr;

	return minEdge;
}

float Polygon::GetDistanceToEdge(glm::vec2 point, Polygon::Edge* edge)
{
 	glm::vec2 vEdge = edge->to->vertex - edge->from->vertex;
	glm::vec2 oEdge = glm::vec2(-vEdge.y, vEdge.x);

	glm::vec2 vTarget = point - edge->from->vertex;

	// Add check to inversable or not
	glm::vec2 vResult = glm::inverse(glm::mat2x2(vEdge, oEdge)) * vTarget;

	if (vResult.x < 0)
		return GetDistanceToVertex(point, edge->from);
	else if (vResult.x > 1)
		return GetDistanceToVertex(point, edge->to);

	return fabs(vResult.y) * glm::length(oEdge);
}

Polygon::Vertex* Polygon::GetNearestVertex(glm::vec2 point)
{
	float minDistance = (1 << 30);
	Vertex* minVertex = nullptr;

	Vertex* nowVertex = vertexHead;
	do
	{
		if (glm::length(point - nowVertex->vertex) < minDistance)
		{
			minDistance = glm::length(point - nowVertex->vertex);
			minVertex = nowVertex;
		}
		nowVertex = nowVertex->next;
	} while (nowVertex != vertexHead);
	return minVertex;
}

float Polygon::GetDistanceToVertex(glm::vec2 point, Polygon::Vertex* vertex)
{
	return glm::length(point - vertex->vertex);
}