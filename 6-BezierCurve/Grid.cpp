#include "Grid.h"

Grid::Grid()
{
	transform = glm::mat3x3(1.0f);

	project = glm::mat3x3(
		2.0f / SCREEN_WIDTH, 0.0f, 0.0f,
		0.0f, -2.0f / SCREEN_HEIGHT, 0.0f,
		-1.0f, 1.0f, 1.0f);

	anchor = glm::vec2(0.0f);

	hoveringEdge = nullptr;
	hoveringVertex = nullptr;
	holdingVertex = nullptr;

	polyline = new Polyline();
	polyline->InitialLine(300);

	curve = nullptr;
}

Grid& Grid::Instance()
{
	static Grid instance;
	return instance;
}

void Grid::Translate(glm::vec2 deltaPosition)
{
	glm::mat3x3 translationMatrix = glm::mat3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		deltaPosition.x, deltaPosition.y, 1.0f);
	transform = translationMatrix * transform;
}

void Grid::Reanchor(glm::vec2 anchor)
{
	Translate(this->anchor - anchor);
	this->anchor = anchor;
}

void Grid::HandleLeftMouseDown(glm::vec2 position)
{
	if (hoveringEdge != nullptr)
	{
		position = InverseTransform(position);
		position.x = (int)roundf(position.x);
		position.y = (int)roundf(position.y);
		holdingVertex = polyline->CreateNewVertexOn(position, hoveringEdge);
		hoveringEdge = nullptr;
	}
	else if (hoveringVertex != nullptr)
	{
		holdingVertex = hoveringVertex;
	}

	OnMouseEventUpdate();
}

void Grid::HandleRightMouseDown(glm::vec2 position)
{
	if (hoveringVertex != nullptr)
	{
		polyline->RemoveVertex(hoveringVertex);
		hoveringVertex = nullptr;
	}

	OnMouseEventUpdate();
}

void Grid::HandleMouseUp(glm::vec2 position)
{
	holdingVertex = nullptr;
}

void Grid::HandleMouseDrag(glm::vec2 position)
{
	position = InverseTransform(position);
	position.x = (int)roundf(position.x);
	position.y = (int)roundf(position.y);

	if (holdingVertex != nullptr && position != holdingVertex->vertex)
		holdingVertex->vertex = position;

	OnMouseEventUpdate();
}

void Grid::HandleMouseMove(glm::vec2 position)
{
	hoveringVertex = nullptr;
	hoveringEdge = nullptr;

	float dist;
	float distFix = (transform[0][0] + transform[1][1]) / 2 / 10;
	position = InverseTransform(position);

	Polyline::Vertex* nearestVertex = polyline->GetNearestVertex(position);
	dist = polyline->GetDistanceToVertex(position, nearestVertex);
	if (dist <= VERTEX_SELECT_DIST / distFix)
	{
		hoveringVertex = nearestVertex;
		return;
	}

	Polyline::Edge* nearestEdge = polyline->GetNearestEdge(position);
	dist = polyline->GetDistanceToEdge(position, nearestEdge);
	if (dist <= EDGE_SELECT_DIST / distFix)
	{
		hoveringEdge = nearestEdge;
		return;
	}
}

void Grid::HandleButtonEvent()
{

}

void Grid::OnMouseEventUpdate()
{
	curve = new BezierCurve(polyline, BEZIER_CURVE_PRECISION);
}

glm::vec2 Grid::Transform(glm::vec2 src)
{
	return transform * glm::vec3(src.x, src.y, 1.0f) + glm::vec3(anchor, 0.0f);
}

glm::vec2 Grid::InverseTransform(glm::vec2 src)
{
	glm::mat3x3 inverseTransform = glm::inverse(transform);
	return inverseTransform * (glm::vec3(src.x, src.y, 1.0f) - glm::vec3(anchor, 0.0f));
}

glm::vec2 Grid::ProjectToScreen(glm::vec2 src)
{
	return project * glm::vec3(src.x, src.y, 1.0f);
}

void Grid::Render()
{
	glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (curve != nullptr)
	{
		RenderLine(curve, CURVE_LINE_WIDTH, CURVE_COLOR);
	}

	RenderPolyline(polyline);
}

void Grid::RenderVerticalLine(int lineWidth, glm::vec2 fromPos, glm::vec2 toPos)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	fromPos = ProjectToScreen(Transform(fromPos + glm::vec2(0, -floorf(lineWidth / 2.0f))));
	toPos = ProjectToScreen(Transform(toPos + glm::vec2(0, lineWidth / 2.0f)));
	glVertex2f(fromPos.x, fromPos.y);
	glVertex2f(toPos.x, toPos.y);
	glEnd();
}

void Grid::RenderHorizontalLine(int lineWidth, glm::vec2 fromPos, glm::vec2 toPos)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	fromPos = ProjectToScreen(Transform(fromPos + glm::vec2(-lineWidth / 2.0f, 0)));
	toPos = ProjectToScreen(Transform(toPos + glm::vec2(lineWidth / 2.0f, 0)));
	glVertex2f(fromPos.x, fromPos.y);
	glVertex2f(toPos.x, toPos.y);
	glEnd();
}

void Grid::RenderPolyline(const Polyline* polyline, bool closed)
{
	RenderLine(polyline, LINE_WIDTH, POLYGON_EDGE_COLOR);

	RenderHoveringEdge();

	RenderHoveringVertex();
}

void Grid::RenderLine(const Polyline* polyline, float width, glm::vec3 color, bool closed)
{
	glLineWidth(width);
	glColor3f(color.r, color.g, color.b);

	if (closed) glBegin(GL_LINE_LOOP);
	else glBegin(GL_LINE_STRIP);

	Polyline::Vertex* vertex = polyline->GetVertexHead();
	if (vertex != nullptr)
	{
		while (vertex != nullptr)
		{
			glm::vec2 point = ProjectToScreen(Transform(vertex->vertex));
			glVertex2f(point.x, point.y);
			vertex = vertex->next;
		}
	}
	glEnd();
}

void Grid::RenderHoveringEdge()
{
	if (hoveringEdge != nullptr)
	{
		glLineWidth(HOVERING_LINE_WIDTH);
		glColor3f(POLYGON_HOVER_COLOR.r, POLYGON_HOVER_COLOR.g, POLYGON_HOVER_COLOR.b);
		glBegin(GL_LINES);
		glm::vec2 from = ProjectToScreen(Transform(hoveringEdge->from->vertex));
		glVertex2f(from.x, from.y);
		glm::vec2 to = ProjectToScreen(Transform(hoveringEdge->to->vertex));
		glVertex2f(to.x, to.y);
		glEnd();
	}
}

void Grid::RenderHoveringVertex()
{
	if (hoveringVertex != nullptr)
	{
		RenderCircleGizmo(hoveringVertex->vertex, POLYGON_HOVER_COLOR, HOVERING_VERTEX_RADIUS, true);
	}
}

void Grid::RenderCircleGizmo(glm::vec2 position, glm::vec3 color, float radius, bool filled)
{
	glColor3f(color.r, color.g, color.b);

	if (filled)
	{
		glBegin(GL_POLYGON);
	}
	else
	{
		glLineWidth(LINE_WIDTH);
		glBegin(GL_LINE_LOOP);
	}

	int number = 30;
	glm::vec2 pPoint = ProjectToScreen(Transform(position));
	float pRadius = radius * GIZMO_SCALE;
	for (int i = 0; i < number; i++)
	{
		glm::vec2 point = glm::vec2(pRadius / SCREEN_WIDTH * cosf(2 * i * PI / number),
			pRadius / SCREEN_HEIGHT * sinf(2 * i * PI / number)) + pPoint;
		glVertex2f(point.x, point.y);
	}
	glEnd();
}