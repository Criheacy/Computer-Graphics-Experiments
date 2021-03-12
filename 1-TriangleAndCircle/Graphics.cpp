#include "Graphics.h"

#define PI acosf(-1)

/******************** Graphics Class ********************/
Graphics::Graphics()
{
    transform = glm::mat3x3(1.0f);

    project = glm::mat3x3(
        2.0f / SCREEN_WIDTH, 0.0f, 0.0f,
        0.0f, -2.0f / SCREEN_HEIGHT, 0.0f,
        -1.0f, 1.0f, 1.0f);

    anchor = glm::vec2(0.0f, 0.0f);
}

void Graphics::Translate(glm::vec2 deltaPosition)
{
    glm::mat3x3 translationMatrix = glm::mat3x3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        deltaPosition.x, deltaPosition.y, 1.0f);
    transform = translationMatrix * transform;
}

void Graphics::Rotate(float deltaAngle)
{
    float arc = deltaAngle * PI / 180;
    glm::mat3x3 rotationMatrix = glm::mat3x3(
        cosf(arc), sinf(arc), 0.0f,
        -sinf(arc), cosf(arc), 0.0f,
        0.0f, 0.0f, 1.0f);
    transform = rotationMatrix * transform;
}

void Graphics::MoveAnchor(glm::vec2 deltaPosition)
{
    anchor += deltaPosition;
}

void Graphics::Reanchor()
{
    anchor += glm::vec2(transform[2].x, transform[2].y);
    transform[2].x = 0.0f;
    transform[2].y = 0.0f;
}

bool Graphics::InObject(glm::vec2 src)
{
    return false;
}

std::vector<glm::vec3> Graphics::GetVertices()
{
    return std::vector<glm::vec3>();
}

glm::vec3 Graphics::GetColor()
{
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

int Graphics::GetRenderMode()
{
    return GL_POLYGON;
}

glm::vec2 Graphics::Transform(glm::vec2 src)
{
    return transform * glm::vec3(src.x, src.y, 1.0f) + glm::vec3(anchor, 0.0f);
}

glm::vec2 Graphics::InverseTransform(glm::vec2 src)
{
    glm::mat3x3 inverseTransform = glm::inverse(transform);

    return inverseTransform * (glm::vec3(src.x, src.y, 1.0f) - glm::vec3(anchor, 0.0f));
}

glm::vec2 Graphics::ProjectToScreen(glm::vec2 src)
{
    return project * glm::vec3(src.x, src.y, 1.0f);
}

/******************** Triangle Class ********************/

Triangle::Triangle(float a)
{
    Graphics();
    float sq3 = sqrtf(3);
    pointList.push_back(glm::vec2(a / 2, -sq3 * a / 6));
    pointList.push_back(glm::vec2(-a / 2, -sq3 * a / 6));
    pointList.push_back(glm::vec2(0, sq3 * a / 3));
}

bool Triangle::InObject(glm::vec2 src)
{
    // Use U-V formula to detect if point is in the triangle
    glm::vec3 CA = glm::vec3(glm::vec2(pointList[2] - pointList[0]), 0.0f);
    glm::vec3 BA = glm::vec3(glm::vec2(pointList[1] - pointList[0]), 0.0f);
    glm::vec3 PA = glm::vec3(glm::vec2(InverseTransform(src) - pointList[0]), 0.0f);

    float u = (glm::dot(glm::cross(BA, BA), glm::cross(PA, CA)) - glm::dot(glm::cross(BA, CA), glm::cross(PA, BA)))
        / (glm::dot(glm::cross(CA, CA), glm::cross(BA, BA)) - glm::dot(glm::cross(CA, BA), glm::cross(BA, CA)));

    float v = (glm::dot(glm::cross(CA, CA), glm::cross(PA, BA)) - glm::dot(glm::cross(CA, BA), glm::cross(PA, CA)))
        / (glm::dot(glm::cross(CA, CA), glm::cross(BA, BA)) - glm::dot(glm::cross(CA, BA), glm::cross(BA, CA)));

    return (u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1);
}

std::vector<glm::vec3> Triangle::GetVertices()
{
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    for (int i = 0; i < (int)pointList.size(); i++)
    {
        glm::vec2 transformedPoint = ProjectToScreen(Transform(pointList[i]));
        vertices.push_back(glm::vec3(transformedPoint, 0.0f));
    }
    return vertices;
}

glm::vec3 Triangle::GetColor()
{
    return glm::vec3(1.0f, 0.733f, 0.02f);
}

/******************** Circle Class ********************/

Circle::Circle(float r)
{
    radius = r;
    center = glm::vec2(0.0f, 0.0f);
}

bool Circle::InObject(glm::vec2 src)
{
    glm::vec2 PO = InverseTransform(src) - center;
    return glm::length(PO) <= radius;
}

std::vector<glm::vec3> Circle::GetVertices()
{
    int number = 360;

    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    for (int i = 0; i < number; i++)
    {
        glm::vec2 point = glm::vec2(radius * cosf(2 * i * PI / number),
            radius * sinf(2 * i * PI / number));

        glm::vec2 transformedPoint = ProjectToScreen(Transform(point));
        vertices.push_back(glm::vec3(transformedPoint, 0.0f));
    }
    return vertices;
}

glm::vec3 Circle::GetColor()
{
    return glm::vec3(0.02f, 0.733f, 1.0f);
}