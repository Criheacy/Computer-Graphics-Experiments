#include "Grid.h"

Grid::Grid()
{
    transform = glm::mat3x3(1.0f);

    project = glm::mat3x3(
        2.0f / SCREEN_WIDTH, 0.0f, 0.0f,
        0.0f, -2.0f / SCREEN_HEIGHT, 0.0f,
        -1.0f, 1.0f, 1.0f);

    anchor = glm::vec2(0.0f);

    showScanningLine = false;
    scanningLine = glm::vec2(0.0f, 0.0f);

    hoveringEdge = nullptr;
    hoveringVertex = nullptr;
    holdingVertex = nullptr;

    waitingList = std::queue<glm::vec2>();
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

void Grid::Scale(float scale)
{
    glm::mat3x3 scalingMatrix = glm::mat3x3(
        scale, 0.0f, 0.0f,
        0.0f, scale, 0.0f,
        0.0f, 0.0f, 1.0f);
    if (ENABLE_SCALE_CONSTRAINT)
    {
        float nowScale = (transform[0][0] + transform[1][1]) / 2;
        if (nowScale * scale >= SCALE_CONSTRAINT_MAX)
        {
            nowScale = SCALE_CONSTRAINT_MAX;
            transform[0][0] = transform[1][1] = nowScale;
        }
        else if (nowScale * scale <= SCALE_CONSTRAINT_MIN)
        {
            nowScale = SCALE_CONSTRAINT_MIN;
            transform[0][0] = transform[1][1] = nowScale;
        }
        else transform = scalingMatrix * transform;
    }
    else
    {
        transform = scalingMatrix * transform;
    }
}

void Grid::ScaleAt(float scale, glm::vec2 scalingPoint)
{
    Reanchor(scalingPoint);
    Scale(scale);
}

void Grid::Reanchor(glm::vec2 anchor)
{
    Translate(this->anchor - anchor);
    this->anchor = anchor;
}

void Grid::HandleLeftMouseDown(glm::vec2 position)
{
    if (animator.IsRunning()) return;
    if (hoveringEdge != nullptr)
    {
        position = InverseTransform(position);
        position.x = (int)roundf(position.x);
        position.y = (int)roundf(position.y);
        holdingVertex = polygon.CreateNewVertexOn(position, hoveringEdge);
        hoveringEdge = nullptr;
        ClearPoints();
    }
    else if (hoveringVertex != nullptr)
    {
        holdingVertex = hoveringVertex;
    }
}

void Grid::HandleRightMouseDown(glm::vec2 position)
{
    if (animator.IsRunning()) return;
    if (hoveringVertex != nullptr)
    {
        polygon.RemoveVertex(hoveringVertex);
        hoveringVertex = nullptr;
        ClearPoints();
    }
}

void Grid::HandleMouseUp(glm::vec2 position)
{
    holdingVertex = nullptr;
    if (animator.IsRunning()) return;
}

void Grid::HandleMouseDrag(glm::vec2 position)
{
    if (holdingVertex == nullptr) return;
    if (animator.IsRunning()) return;

    position = InverseTransform(position);
    position.x = (int)roundf(position.x);
    position.y = (int)roundf(position.y);

    if (position != holdingVertex->vertex)
    {
        holdingVertex->vertex = position;
        ClearPoints();
    }
}

void Grid::HandleMouseMove(glm::vec2 position)
{
    hoveringVertex = nullptr;
    hoveringEdge = nullptr;

    float dist;
    float distFix = (transform[0][0] + transform[1][1]) / 2 / 10;
    position = InverseTransform(position);
    Polygon::Vertex* nearestVertex = polygon.GetNearestVertex(position);
    dist = polygon.GetDistanceToVertex(position, nearestVertex);
    
    if (dist <= VERTEX_SELECT_DIST / distFix)
    {
        hoveringVertex = nearestVertex;
        return;
    }

    Polygon::Edge* nearestEdge = polygon.GetNearestEdge(position);
    dist = polygon.GetDistanceToEdge(position, nearestEdge);

    if (dist <= EDGE_SELECT_DIST / distFix)
    {
        hoveringEdge = nearestEdge;
    }
}

void Grid::HandleButtonEvent()
{
    if (!animator.IsRunning())
    {
        ClearPoints();
        ScanLineOnPolygon();
    }
}

void Grid::ScanLineOnPolygon()
{
    Algorithm::ScanLine(this, &polygon);
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

void Grid::AddPointToWaitingList(int x, int y)
{
    waitingList.push(glm::vec2(x, y));
}

void Grid::StartAnimation()
{
    int pointNumber = waitingList.size();
    animator.SetValueRange(pointNumber, 0);
    int duration = 5000;
    if (pointNumber < 500)
        duration = 10 * pointNumber;
    animator.SetDuration(duration);

    showScanningLine = true;
    animator.StartAnimation();
}

void Grid::UpdateAnimation()
{
    int nowValue = animator.GetValue();
    while (nowValue < (int)waitingList.size())
    {
        if (waitingList.empty()) break;

        glm::vec2 nextNode = waitingList.front();
        if (nextNode.y != scanningLine.y)
        {
            scanningLine.y = nextNode.y;
            showScanningLine = true;
        }

        pointList.push_back(nextNode);
        waitingList.pop();
    }
    if (waitingList.empty())
        showScanningLine = false;
}

void Grid::SetScanLineShownState(bool state)
{
    showScanningLine = state;
}

void Grid::SetScanLinePosition(glm::vec2 position)
{
    scanningLine = position;
}

void Grid::MarkPoint(int x, int y)
{
    pointList.push_back(glm::vec2(x, y));
}

void Grid::ClearPoints()
{
    pointList.clear();
}

glm::vec2 Grid::InPoint(glm::vec2 src)
{
    glm::vec2 inv = InverseTransform(src);
    return glm::vec2((int)roundf(inv.x), (int)roundf(inv.y));
}

int Grid::ConstrainScale()
{
    if (transform[0][0] <= SCALE_CONSTRAINT_MIN || transform[1][1] <= SCALE_CONSTRAINT_MIN)
    {
        transform[0][0] = SCALE_CONSTRAINT_MIN;
        transform[1][1] = SCALE_CONSTRAINT_MIN;
        return -1;
    }
    if (transform[0][0] >= SCALE_CONSTRAINT_MAX || transform[1][1] >= SCALE_CONSTRAINT_MAX)
    {
        transform[0][0] = SCALE_CONSTRAINT_MAX;
        transform[1][1] = SCALE_CONSTRAINT_MAX;
        return 1;
    }
    return 0;
}

void Grid::Render()
{
    glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (showScanningLine)
        RenderScanningLine();
    RenderPoints();

    if ((transform[0][0] + transform[1][1]) / 2 >= FADE_FRAME_LINES_WHEN_SCALE)
    {
        RenderGridLines();
    }

    RenderPolygon();
}

void Grid::RenderPoints()
{
    glm::vec2 dist;
    float scaleX = 2.0f * transform[0][0] / SCREEN_WIDTH;
    float scaleY = 2.0f * transform[1][1] / SCREEN_HEIGHT;

    for (auto point = pointList.begin(); point != pointList.end(); point++)
    {
        dist = ProjectToScreen(Transform(glm::vec2((*point).x, (*point).y)));
        if (showScanningLine && (*point).y == scanningLine.y)
            glColor3f(POINT_ON_SCANNING_LINE_COLOR.r, POINT_ON_SCANNING_LINE_COLOR.g, POINT_ON_SCANNING_LINE_COLOR.b);
        else glColor3f(MARK_COLOR.r, MARK_COLOR.g, MARK_COLOR.b);
        glBegin(GL_POLYGON);
        glVertex2f(dist.x - 0.5f * scaleX, dist.y - 0.5f * scaleY);
        glVertex2f(dist.x - 0.5f * scaleX, dist.y + 0.5f * scaleY);
        glVertex2f(dist.x + 0.5f * scaleX, dist.y + 0.5f * scaleY);
        glVertex2f(dist.x + 0.5f * scaleX, dist.y - 0.5f * scaleY);
        glEnd();
    }

}

void Grid::RenderGridLines()
{
    glm::vec2 inv;
    glm::vec2 from, to;
    int fromX, toX, fromY, toY;

    inv = InverseTransform(glm::vec2(-STROKE_WIDTH_MAX, 0.0f));
    fromX = (int)floorf(inv.x);

    inv = InverseTransform(glm::vec2(SCREEN_WIDTH + STROKE_WIDTH_MAX, 0.0f));
    toX = (int)ceilf(inv.x);

    inv = InverseTransform(glm::vec2(0.0f, -STROKE_WIDTH_MAX));
    fromY = (int)floorf(inv.y);

    inv = InverseTransform(glm::vec2(0.0f, SCREEN_WIDTH + STROKE_WIDTH_MAX));
    toY = (int)ceilf(inv.y);

    glLoadIdentity();
    glColor3f(GRID_COLOR.r, GRID_COLOR.g, GRID_COLOR.b);

    float scaleAvg = (transform[0][0] + transform[1][1]) / 2;

    glLineWidth(STROKE_WIDTH_MAX * scaleAvg / SCALE_CONSTRAINT_MAX);

    for (int i = fromX; i <= toX; i++)
    {
        from = ProjectToScreen(Transform(glm::vec2(i - 0.5f, 0)));
        to = ProjectToScreen(Transform(glm::vec2(i - 0.5f, SCREEN_HEIGHT)));
        glBegin(GL_LINES);
        glVertex2f(from.x, -1.0f);
        glVertex2f(to.x, 1.0f);
        glEnd();
    }

    for (int i = fromY; i <= toY; i++)
    {
        from = ProjectToScreen(Transform(glm::vec2(0, i - 0.5f)));
        to = ProjectToScreen(Transform(glm::vec2(SCREEN_HEIGHT, i - 0.5f)));
        glBegin(GL_LINES);
        glVertex2f(-1.0f, from.y);
        glVertex2f(1.0f, to.y);
        glEnd();
    }
}

void Grid::RenderScanningLine()
{
    glm::vec2 dist;
    float scaleX = 2.0f * transform[0][0] / SCREEN_WIDTH;
    float scaleY = 2.0f * transform[1][1] / SCREEN_HEIGHT;

    dist = ProjectToScreen(Transform(glm::vec2(scanningLine.x, scanningLine.y)));
    glColor3f(SCANNING_LINE_COLOR.r, SCANNING_LINE_COLOR.g, SCANNING_LINE_COLOR.b);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, dist.y - 0.5f * scaleY);
    glVertex2f(-1.0f, dist.y + 0.5f * scaleY);
    glVertex2f(1.0f, dist.y + 0.5f * scaleY);
    glVertex2f(1.0f, dist.y - 0.5f * scaleY);
    glEnd();
}

void Grid::RenderPolygon()
{
    glLoadIdentity();
    glColor3f(POLYGON_EDGE_COLOR.r, POLYGON_EDGE_COLOR.g, POLYGON_EDGE_COLOR.b);

    float scaleAvg = (transform[0][0] + transform[1][1]) / 2;
    glLineWidth(ceilf(STROKE_WIDTH_MAX * scaleAvg / SCALE_CONSTRAINT_MAX * 1.5f));

    glBegin(GL_LINE_LOOP);
    Polygon::Vertex* vertex = polygon.GetVertexHead();
    if (vertex != nullptr)
    {
        do
        {
            glm::vec2 point = ProjectToScreen(Transform(vertex->vertex));
            glVertex2f(point.x, point.y);
            vertex = vertex->next;
        } while (vertex != polygon.GetVertexHead());
    }
    glEnd();

    RenderHoveringEdge();

    // Draw gizmo circle
    if (vertex != nullptr)
    {
        do
        {
            RenderCircleGizmo(vertex->vertex, POLYGON_VERTEX_COLOR, 1, true);
            vertex = vertex->next;
        } while (vertex != polygon.GetVertexHead());
    }

    RenderHoveringVertex();
}

void Grid::RenderHoveringEdge()
{
    if (hoveringEdge != nullptr)
    {
        glLineWidth(5);
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
        RenderCircleGizmo(hoveringVertex->vertex, POLYGON_HOVER_COLOR, 1.5f, true);
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
        glLineWidth(1.5f);
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