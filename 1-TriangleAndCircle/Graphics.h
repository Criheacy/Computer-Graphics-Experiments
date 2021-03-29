#pragma once

#include "Settings.h"

#include <vector>
#include <math.h>

#include <glut.h>

class Graphics
{
public:
    Graphics();

    void Translate(glm::vec2 deltaPosition);
    void Rotate(float deltaAngle);

    void MoveAnchor(glm::vec2 deltaPosition);
    void Reanchor();

    // Check if coordinate of point is in graphics object
    virtual bool InObject(glm::vec2 src);

    // Provide vertices data for renderer
    virtual std::vector<glm::vec3> GetVertices();

    // Provide render color for renderer
    virtual glm::vec3 GetColor();

    // Provide render mode for renderer
    int GetRenderMode();

    // Convert coordinate from vector to screen position
    glm::vec2 ProjectToScreen(glm::vec2 src);

protected:
    // Convert from origin to coordinate which has been transformed (such as translated or rotated ...)
    glm::vec2 Transform(glm::vec2 src);

    // Get transformed coordinates
    glm::vec2 InverseTransform(glm::vec2 src);

    // Attributes

    glm::vec2 anchor;
    glm::mat3x3 transform;
    glm::mat3x3 project;
};

class Triangle : public Graphics
{
public:
    Triangle(float a);

    bool InObject(glm::vec2 src) override;
    std::vector<glm::vec3> GetVertices() override;
    glm::vec3 GetColor() override;

private:
    std::vector<glm::vec2> pointList;
};

class Circle : public Graphics
{
public:
    Circle(float r);

    bool InObject(glm::vec2 src) override;
    std::vector<glm::vec3> GetVertices() override;
    glm::vec3 GetColor() override;

private:
    glm::vec2 center;
    float radius;
};