#include <glut.h>

#include "Grid.h"
#include "Settings.h"
#include "Animator.hpp"
#include "Algorithm.h"
#include "UIComponents.h"

int mouseX;
int mouseY;
bool leftPressed;
bool middlePressed;
bool rightPressed;
int mouseDragFromX;
int mouseDragFromY;

int frameCount;
int lastTime;

void RenderScene(void)
{
    if (SHOW_SCANNING_ANIMATION)
        Grid::Instance().UpdateAnimation();

    Grid::Instance().Render();
    UI::Canvas::Instance().Render();
    glutSwapBuffers();

    glutPostRedisplay();
}

void Resize(int width, int height)
{
    glutReshapeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void HandleMouseButtonEvent(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (UI::Canvas::Instance().HandleMouseDown(glm::vec2(x, y))) return;

        mouseX = x, mouseY = y;
        mouseDragFromX = x, mouseDragFromY = y;

        if (button == GLUT_LEFT_BUTTON)
        {
            leftPressed = true;
            Grid::Instance().HandleLeftMouseDown(glm::vec2(x, y));
        }
        else if (button == GLUT_MIDDLE_BUTTON)
            middlePressed = true;
        else if (button == GLUT_RIGHT_BUTTON)
        {
            Grid::Instance().HandleRightMouseDown(glm::vec2(x, y));
            rightPressed = true;
        }
    }
    else if (state == GLUT_UP)
    {
        if (UI::Canvas::Instance().HandleMouseUp(glm::vec2(x, y))) return;

        if (button == GLUT_LEFT_BUTTON)
        {
            leftPressed = false;
            Grid::Instance().HandleMouseUp(glm::vec2(x, y));
        }
        else if (button == GLUT_MIDDLE_BUTTON)
            middlePressed = false;
        else if (button == GLUT_RIGHT_BUTTON)
            rightPressed = false;
    }
}

void HandleMouseMotionEvent(int x, int y)
{
    if (UI::Canvas::Instance().HandleMotion(glm::vec2(x, y))) return;

    if (leftPressed)
    {
        Grid::Instance().HandleMouseDrag(glm::vec2(x, y));
    }
    if (middlePressed)
    {
        float scale = (x - mouseX + y - mouseY) * 0.01;
        Grid::Instance().ScaleAt(1.0f + scale, glm::vec2(mouseDragFromX, mouseDragFromY));
        mouseX = x, mouseY = y;
    }
    if (rightPressed)
    {
        Grid::Instance().Translate(glm::vec2(x - mouseX, y - mouseY));
        mouseX = x, mouseY = y;
    }
}

void HandleMousePassiveMotionEvent(int x, int y)
{
    UI::Canvas::Instance().HandleMotion(glm::vec2(x, y));
    Grid::Instance().HandleMouseMove(glm::vec2(x, y));
}

int main(int argc, char* argv[])
{
    glutInit(&argc, (char**)argv);

    // Initialize and create window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("3 - Scan Line");

    // Scaling grid background
    Grid::Instance().ScaleAt(18, glm::vec2(-20, -17));

    // Declear buttons on UI canvas
    UI::Button applyAlgorithm = UI::Button(2, SCREEN_WIDTH - 130, 30, SCREEN_WIDTH - 30, 60);
    applyAlgorithm.SetText("SCAN LINE");
    applyAlgorithm.SetOnSelectListener([]() -> void {
        Grid::Instance().HandleButtonEvent();
    });

    UI::Canvas::Instance().AddComponent(&applyAlgorithm);

    lastTime = glutGet(GLUT_ELAPSED_TIME);

    // Add callback functions
    glutReshapeFunc(Resize);
    glutDisplayFunc(RenderScene);
    glutMouseFunc(HandleMouseButtonEvent);
    glutMotionFunc(HandleMouseMotionEvent);
    glutPassiveMotionFunc(HandleMousePassiveMotionEvent);

    // Enter main loop
    glutMainLoop();
    return 0;
}