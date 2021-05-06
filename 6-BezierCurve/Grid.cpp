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

	polygon = new Polygon();
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
		holdingVertex = polygon->CreateNewVertexOn(position, hoveringEdge);
		hoveringEdge = nullptr;
	}
	else if (hoveringVertex != nullptr)
	{
		holdingVertex = hoveringVertex;
	}
}

void Grid::HandleRightMouseDown(glm::vec2 position)
{
	if (hoveringVertex != nullptr)
	{
		polygon->RemoveVertex(hoveringVertex);
		hoveringVertex = nullptr;
	}
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
}

void Grid::HandleMouseMove(glm::vec2 position)
{
	hoveringVertex = nullptr;
	hoveringEdge = nullptr;

	float dist;
	float distFix = (transform[0][0] + transform[1][1]) / 2 / 10;
	position = InverseTransform(position);

	Polygon::Vertex* nearestVertex = polygon->GetNearestVertex(position);
	dist = polygon->GetDistanceToVertex(position, nearestVertex);
	if (dist <= VERTEX_SELECT_DIST / distFix)
	{
		hoveringVertex = nearestVertex;
		return;
	}

	Polygon::Edge* nearestEdge = polygon->GetNearestEdge(position);
	dist = polygon->GetDistanceToEdge(position, nearestEdge);
	if (dist <= EDGE_SELECT_DIST / distFix)
	{
		hoveringEdge = nearestEdge;
		return;
	}
}

void Grid::HandleButtonEvent()
{

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

	RenderPolyline(polygon);
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

/*********** CALLBACK FUNCTIONS ***********/

void CALLBACK vertexCallback(GLvoid* vertex)
{
	GLdouble* pt = (GLdouble*)vertex;
	glVertex3d(pt[0], pt[1], pt[2]);
}

void CALLBACK beginCallback(GLenum type)
{
	glBegin(type);
}

void CALLBACK endCallback()
{
	glEnd();
}

void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte* estring;
	estring = gluErrorString(errorCode);
//	fprintf(stderr, "Tessellation Error: %s\n", estring);
	// exit(0);
}

void Grid::RenderPolygon(const Polygon* polygon, glm::vec3 fillColor)
{
	glColor3f(fillColor.r, fillColor.g, fillColor.b);
	glLineWidth(LINE_WIDTH);

	int cnt = 0;
	glBegin(GL_POLYGON);
	glEnd();

	GLUtesselator* tess = gluNewTess();

	gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK*)())beginCallback);
	gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK*)())endCallback);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK*)())vertexCallback);
	gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK*)())errorCallback);

	Polygon::Vertex* vertex = polygon->GetVertexHead();
	std::unique_ptr<double*> vertexList(new double* [polygon->GetVertexNumber() + 1]);
	int vertexCnt = 0;

	gluTessBeginPolygon(tess, nullptr);
	gluTessBeginContour(tess);

	if (vertex != nullptr)
	{
		do
		{
			glm::vec2 point = ProjectToScreen(Transform(vertex->vertex));
			double* vertexValue = new double[3];
			vertexValue[0] = point.x;
			vertexValue[1] = point.y;
			vertexValue[2] = 0;
			vertexList.get()[cnt++] = vertexValue;

				gluTessVertex(tess, vertexValue, vertexValue);
			vertex = vertex->next;
		} while (vertex != polygon->GetVertexHead());
	}

	gluTessEndContour(tess);
	gluTessEndPolygon(tess);

	for (int i = 0; i < polygon->GetVertexNumber(); i++)
		delete[] vertexList.get()[i];

	glColor3f(POLYGON_EDGE_COLOR.r, POLYGON_EDGE_COLOR.g, POLYGON_EDGE_COLOR.b);
	glBegin(GL_LINE_LOOP);
	vertex = polygon->GetVertexHead();
	if (vertex != nullptr)
	{
		do
		{
			glm::vec2 point = ProjectToScreen(Transform(vertex->vertex));
			glVertex2f(point.x, point.y);
			vertex = vertex->next;
		} while (vertex != polygon->GetVertexHead());
	}
	glEnd();

	RenderHoveringEdge();

	RenderHoveringVertex();
}

void Grid::RenderPolyline(const Polygon* polygon, bool closed)
{
	glLineWidth(LINE_WIDTH);
	glColor3f(POLYGON_EDGE_COLOR.r, POLYGON_EDGE_COLOR.g, POLYGON_EDGE_COLOR.b);

	if (closed) glBegin(GL_LINE_LOOP);
	else glBegin(GL_LINE_STRIP);

	Polygon::Vertex* vertex = polygon->GetVertexHead();
	if (vertex != nullptr)
	{
		do
		{
			glm::vec2 point = ProjectToScreen(Transform(vertex->vertex));
			glVertex2f(point.x, point.y);
			vertex = vertex->next;
		} while (vertex != polygon->GetVertexHead());
	}
	glEnd();

	RenderHoveringEdge();

	RenderHoveringVertex();
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