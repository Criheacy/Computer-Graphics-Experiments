#include "Settings.h"
#include "Grid.h"

Grid::Grid()
{
    transform = glm::mat3x3(1.0f);

    project = glm::mat3x3(
        2.0f / SCREEN_WIDTH, 0.0f, 0.0f,
        0.0f, -2.0f / SCREEN_HEIGHT, 0.0f,
        -1.0f, 1.0f, 1.0f);

    pointList = std::vector<glm::vec2>();
    hoveringPoint = glm::vec2(-1.0f, -1.0f);

    startPoint = glm::vec2(-1.0f, -1.0f);
    endPoint = glm::vec2(-1.0f, -1.0f);

    anchor = glm::vec2(0.0f);
    shapeType = SHAPE_NONE;
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
        if (ConstrainScale() * (scale - 1.0f) <= 0)
            transform = scalingMatrix * transform;
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

void Grid::SetStartPoint(glm::vec2 point)
{
    startPoint = point;
    endPoint = point;
    ClearPoints();
    shapeType = SHAPE_LINE;
}

void Grid::SetEndPoint(glm::vec2 point)
{
    if (point != endPoint)
    {
        endPoint = point;
        ClearPoints();
        Algorithm::BresenhamLine(this, startPoint.x, endPoint.x, startPoint.y, endPoint.y);
    }
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

void Grid::SetHoverPoint(glm::vec2 point)
{
    hoveringPoint = point;
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

    RenderPoints();

    if ((transform[0][0] + transform[1][1]) / 2 >= FADE_FRAME_LINES_WHEN_SCALE)
    {
        RenderGridLines();
        if (shapeType == SHAPE_LINE)
        {
            RenderAuxiliaryLine();
        }
    }

    glutSwapBuffers();
}

void Grid::RenderPoints()
{
    glm::vec2 dist;
    float scaleX = 2.0f * transform[0][0] / SCREEN_WIDTH;
    float scaleY = 2.0f * transform[1][1] / SCREEN_HEIGHT;

    glLoadIdentity();

    if (ENABLE_HOVERING_HINT)
    {
        glColor3f(HOVER_COLOR.r, HOVER_COLOR.g, HOVER_COLOR.b);
        dist = ProjectToScreen(Transform(glm::vec2(hoveringPoint.x, hoveringPoint.y)));
        glBegin(GL_POLYGON);
        glVertex2f(dist.x - 0.5f * scaleX, dist.y - 0.5f * scaleY);
        glVertex2f(dist.x - 0.5f * scaleX, dist.y + 0.5f * scaleY);
        glVertex2f(dist.x + 0.5f * scaleX, dist.y + 0.5f * scaleY);
        glVertex2f(dist.x + 0.5f * scaleX, dist.y - 0.5f * scaleY);
        glEnd();
    }

    glColor3f(MARK_COLOR.r, MARK_COLOR.g, MARK_COLOR.b);

    for (auto point = pointList.begin(); point != pointList.end(); point++)
    {
        dist = ProjectToScreen(Transform(glm::vec2((*point).x, (*point).y)));
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

    //printf("X:[%d-%d] Y:[%d-%d]\n", fromX, toX, fromY, toY);

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

void Grid::RenderAuxiliaryLine()
{
    glLoadIdentity();
    glColor3f(LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b);

    glm::vec2 from = ProjectToScreen(Transform(startPoint));
    glm::vec2 to = ProjectToScreen(Transform(endPoint));

    glBegin(GL_LINES);
    glVertex2f(from.x, from.y);
    glVertex2f(to.x, to.y);
    glEnd();
}