#include "Graphics.h"

#define PI acosf(-1)

/******************** Graphics Class ********************/
Graphics::Graphics()
{
    transform = glm::mat3x3(1.0f);

    /*project = glm::mat3x3(
        2.0f / SCREEN_WIDTH, 0.0f, -1.0f,
        0.0f, -2.0f / SCREEN_HEIGHT, 1.0f,
        0.0f, 0.0f, 1.0f);*/

    project = glm::mat3x3(
        2.0f / SCREEN_WIDTH, 0.0f, 0.0f,
        0.0f, -2.0f / SCREEN_HEIGHT, 0.0f,
        -1.0f, 1.0f, 1.0f);

    /*project = glm::mat3x3(
        1.0f / SCREEN_WIDTH, 0.0f, 0.0f,
        0.0f, -1.0f / SCREEN_HEIGHT, 0.0f,
        0.0f, 0.0f, 1.0f);*/
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

bool Graphics::InObject(glm::vec2 src)
{
    return false;
}

std::vector<glm::vec3> Graphics::GetVertices()
{
    return std::vector<glm::vec3>();
}

int Graphics::GetRenderMode()
{
    return GL_POLYGON;
}

glm::vec2 Graphics::Transform(glm::vec2 src)
{
    return transform * glm::vec3(src.x, src.y, 1.0f);
}

glm::vec2 Graphics::InverseTransform(glm::vec2 src)
{
    glm::mat3x3 inverseTransform = glm::inverse(transform);

    return inverseTransform * glm::vec3(src.x, src.y, 1.0f);
}

glm::vec2 Graphics::ProjectToScreen(glm::vec2 src)
{
    project[1].x;
    return project * glm::vec3(src.x, src.y, 1.0f);
}

glm::vec2 Graphics::ConvertToCoordinate(glm::vec2 src)
{
    glm::mat3x3 inverseProject = glm::inverse(project);

    return inverseProject * glm::vec3(src.x, src.y, 1.0f);
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

    if ((u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1))
    {
        printf("Triangle FLAG\n");
    }

    return (u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1);
}

std::vector<glm::vec3> Triangle::GetVertices()
{
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    for (int i = 0; i < pointList.size(); i++)
    {
        glm::vec2 transformedPoint = ProjectToScreen(Transform(pointList[i]));
        vertices.push_back(glm::vec3(transformedPoint, 0.0f));
    }
    return vertices;
}

glm::vec2 Triangle::Transform(glm::vec2 src)
{
    //printf("[%6.2f, %6.2f] --Transform-> [%6.2f, %6.2f]\n", src.x, src.y,
    //    (transform * glm::vec3(src.x, src.y, 1.0f)).x, (transform * glm::vec3(src.x, src.y, 1.0f)).y);
    return transform * glm::vec3(src.x, src.y, 1.0f);
}

glm::vec2 Triangle::ProjectToScreen(glm::vec2 src)
{
    /*printf("[%6.2f, %6.2f] --Project-> [%6.2f, %6.2f]\n", src.x, src.y,
        (project * glm::vec3(src.x, src.y, 1.0f)).x, (project * glm::vec3(src.x, src.y, 1.0f)).y);*/
    return project * glm::vec3(src.x, src.y, 1.0f);
}

/******************** Circle Class ********************/

Circle::Circle(float r)
{
    radius = r;
    center = glm::vec2(0.0f, 0.0f);
}

bool Circle::InObject(glm::vec2 src)
{
    /*printf("[%6.2f, %6.2f] --INV-Project-> [%6.2f, %6.2f]\n", src.x, src.y,
        InverseTransform(src).x, InverseTransform(src).y);*/
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