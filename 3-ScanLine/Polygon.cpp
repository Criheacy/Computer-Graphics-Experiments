#include "Polygon.h"

Polygon::Polygon()
{
	InitialTriangle(10);
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
	vertexList.push_back(top);
	vertexList.push_back(left);
	vertexList.push_back(right);
}

void Polygon::ClearVertex()
{
	for (auto vertex : vertexList)
	{
		delete vertex;
		vertex = nullptr;
	}
	vertexList.clear();
}

Polygon::Vertex* Polygon::operator[](int i)
{
	return vertexList[i];
}

Polygon::Vertex* Polygon::GetFirstVertex()
{
	return vertexList[0];
}

void Polygon::CreateNewVertexOn(glm::vec2 position, Edge* attachedEdge)
{
	Vertex* fromVertex = attachedEdge->from;
	Vertex* toVertex = attachedEdge->to;
	if (fromVertex->next != attachedEdge->to)
		std::swap(fromVertex, toVertex);
	if (fromVertex->next != attachedEdge->to)
	{
		printf("[LOG] In 'CreateNewVertexOn': Invalid Edge\n");
		return;
	}

	Vertex* newVertex = new Vertex{ position, fromVertex->next };
	fromVertex->next = newVertex;
	vertexList.push_back(newVertex);
}

void Polygon::RemoveVertex(Vertex* vertex, bool force)
{
	if (vertexList.empty()) return;
	if ((vertexList.size() <= 3) && !force) return;

	Vertex* preVertex = *(vertexList.begin());
	for (auto nowVertex = vertexList.begin(); nowVertex != vertexList.end(); nowVertex++)
	{
		if (*nowVertex == vertex)
		{
			preVertex->next = (*nowVertex)->next;
			vertexList.erase(nowVertex);
			break;
		}
		preVertex = (*nowVertex);
	}
}

Polygon::Edge* Polygon::GetNearestEdge(glm::vec2 point)
{
	float minDistance = (1 << 30);
	Edge* minEdge = new Edge{ nullptr, nullptr };
	Edge* nowEdge = new Edge{ nullptr, nullptr };

	if (vertexList.size() < 2)
		return nullptr;

	for (auto vertex = vertexList.begin(); vertex != vertexList.end(); vertex++)
	{
		nowEdge->from = *vertex;
		nowEdge->to = (*vertex)->next;
		float dist = GetDistanceToEdge(point, nowEdge);
		if (dist < minDistance)
		{
			minDistance = dist;
			minEdge->from = nowEdge->from;
			minEdge->to = nowEdge->to;
		}

		printf("%.2f, %.2f\n", dist, minDistance);
	}

	printf("Min Edge: (%.2f, %.2f) (%.2f, %.2f)", minEdge->from->vertex.x, minEdge->from->vertex.y,
		minEdge->to->vertex.x, minEdge->to->vertex.y);

	return minEdge;
}

float Polygon::GetDistanceToEdge(glm::vec2 point, Polygon::Edge* edge)
{
	glm::vec2 vEdge = edge->to->vertex - edge->from->vertex;
	glm::vec2 oEdge = glm::vec2(-vEdge.y, vEdge.x);

	glm::vec2 vTarget = point - edge->from->vertex;

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
	for (auto vertex = vertexList.begin(); vertex != vertexList.end(); vertex++)
	{
		if (glm::length(point - (*vertex)->vertex) < minDistance)
		{
			minDistance = glm::length(point - (*vertex)->vertex);
			minVertex = *vertex;
		}
	}
	return minVertex;
}

float Polygon::GetDistanceToVertex(glm::vec2 point, Polygon::Vertex* vertex)
{
	return glm::length(point - vertex->vertex);
}