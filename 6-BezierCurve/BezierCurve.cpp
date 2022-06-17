#include "BezierCurve.h"

BezierCurve::BezierCurve(const Polyline* polyline, int precision)
{
	std::vector<glm::vec2> pointList;

	for (int fraction = 0; fraction <= precision; fraction++)
	{
		pointList.push_back(GetCurvePointByFormula(polyline, fraction * 1.0f / precision));
	}

	InitialVerticesList(pointList);
}

glm::vec2 BezierCurve::GetCurvePoint(const Polyline* polyline, float t)
{
	Polyline* curvePoint = new Polyline(*polyline);
	while (curvePoint->GetVertexNumber() > 1)
	{
		AscendCurveOrder(curvePoint, t);
	}
	glm::vec2 res = curvePoint->GetVertexHead()->vertex;
	delete curvePoint;
	return res;
}

glm::vec2 BezierCurve::GetCurvePointByFormula(const Polyline* polyline, float t)
{
	Vertex* vertex = polyline->GetVertexHead();
	int n = polyline->GetVertexNumber();

	glm::vec2 res = glm::vec2(0.0f);
	for (int i = 1; i <= n; ++i)
	{
		res += static_cast<float>(util::C(n, i) * pow(t, i) * pow(1 - t, n - i)) * vertex->vertex;
		vertex = vertex->next;
	}
	return res;
}

void BezierCurve::AscendCurveOrder(Polyline* polyline, float t)
{
	Vertex* vertex = polyline->GetVertexHead();
	Vertex* preVertex = nullptr;
	if (vertex == nullptr) return;

	// Update each vertex as the interpolation between the next point and itself
	while (vertex->next != nullptr)
	{
		vertex->vertex = Interpolate(vertex->vertex, vertex->next->vertex, t);
		preVertex = vertex;
		vertex = vertex->next;
	}

	// Delete last point (use force mode)
	polyline->RemoveVertex(vertex, true);
}

glm::vec2 BezierCurve::Interpolate(glm::vec2 from, glm::vec2 to, float t)
{
	return from * t + to * (1 - t);
}