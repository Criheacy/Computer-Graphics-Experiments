#include "Polyline.h"

Polyline::Polyline()
{
	vertexHead = nullptr;
	vertexNumber = 0;
}

Polyline::Polyline(const Polyline& polyline)
{
	this->vertexNumber = polyline.GetVertexNumber();
	this->vertexHead = new Vertex{ polyline.GetVertexHead()->vertex, nullptr };

	Vertex* preVertex = this->vertexHead;
	Vertex* vertex = polyline.GetVertexHead()->next;

	while (vertex != nullptr)
	{
		Vertex* newVertex = new Vertex{ vertex->vertex, nullptr };
		preVertex->next = newVertex;
		preVertex = newVertex;
		vertex = vertex->next;
	};

	preVertex->next = nullptr;
}

Polyline::~Polyline()
{
	ClearVertex();
}

Polyline& Polyline::operator=(const Polyline& polyline)
{
	Polyline* newPolyline = new Polyline(polyline);
	return *newPolyline;
}

void Polyline::InitialLine(float a)
{
	ClearVertex();
	Vertex* left = new Vertex{ glm::vec2(-a / 2, 0), nullptr };
	Vertex* right = new Vertex{ glm::vec2(a / 2, 0), nullptr };
	left->next = right;
	right->next = nullptr;

	vertexHead = left;
	vertexNumber = 2;
}

void Polyline::InitialVerticesList(std::vector<glm::vec2> vertices)
{
	ClearVertex();
	vertexHead = nullptr;
	vertexNumber = vertices.size();
	if (vertexNumber == 0)
	{
		vertexHead = nullptr;
		return;
	}

	Vertex* nowVertex = new Vertex{ vertices[0], nullptr };
	vertexHead = nowVertex;

	for (int i = 1; i < vertexNumber; i++)
	{
		nowVertex->next = new Vertex{ vertices[i], nullptr };
		nowVertex = nowVertex->next;
	}
	nowVertex->next = nullptr;
}

void Polyline::ClearVertex()
{
	vertexNumber = 0;
	if (vertexHead == nullptr) return;

	Vertex* nowVertex = vertexHead;
	Vertex* nextVertex = NULL;

	while (nowVertex != NULL)
	{
		nextVertex = nowVertex->next;
		free(nowVertex);
		nowVertex = nextVertex;
	}

	vertexHead = nullptr;
}

Polyline::Vertex* Polyline::GetVertexHead() const
{
	return vertexHead;
}

int Polyline::GetVertexNumber() const
{
	return vertexNumber;
}

Polyline::Vertex* Polyline::CreateNewVertexOn(glm::vec2 position, Edge* attachedEdge)
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

Polyline::Vertex* Polyline::CreateNewVertexAfter(glm::vec2 position, Vertex* preVertex)
{
	Vertex* newVertex = new Vertex{ position, preVertex->next };
	preVertex->next = newVertex;
	vertexNumber++;
	return newVertex;
}

void Polyline::RemoveVertex(Vertex* vertex, bool force)
{
	if (GetVertexNumber() == 0) return;
	if ((GetVertexNumber() <= 2) && !force) return;

	// check to delete head vertex on force mode
	if (force && vertex == vertexHead)
	{
		Vertex* nextVertex = vertexHead->next;
		delete vertexHead;
		vertexHead = nextVertex;
		vertexNumber--;
		return;
	}
	
	Vertex* preVertex = vertexHead;
	Vertex* nowVertex = vertexHead->next;

	while (nowVertex != nullptr)
	{
		// prevent deleting last vertex on non-force mode
		if (!force && nowVertex->next == nullptr) return;

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
	}
}

Polyline::Edge* Polyline::GetNearestEdge(glm::vec2 point)
{
	float minDistance = (1 << 30);
	Edge* minEdge = new Edge{ nullptr, nullptr };
	Edge* nowEdge = new Edge{ nullptr, nullptr };

	if (GetVertexNumber() < 2)
		return nullptr;

	Vertex* nowVertex = vertexHead;
	while (nowVertex != nullptr && nowVertex->next != nullptr)
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
	}

	delete nowEdge;
	nowEdge = nullptr;

	return minEdge;
}

float Polyline::GetDistanceToEdge(glm::vec2 point, Polyline::Edge* edge)
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

Polyline::Vertex* Polyline::GetNearestVertex(glm::vec2 point)
{
	float minDistance = (1 << 30);
	Vertex* minVertex = nullptr;

	Vertex* nowVertex = vertexHead;
	while (nowVertex != nullptr)
	{
		if (glm::length(point - nowVertex->vertex) < minDistance)
		{
			minDistance = glm::length(point - nowVertex->vertex);
			minVertex = nowVertex;
		}
		nowVertex = nowVertex->next;
	}
	return minVertex;
}

float Polyline::GetDistanceToVertex(glm::vec2 point, Polyline::Vertex* vertex)
{
	return glm::length(point - vertex->vertex);
}