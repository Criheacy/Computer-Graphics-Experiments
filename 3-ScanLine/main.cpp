#include <glut.h>

#include "UIComponents.h"
#include "Settings.h"
#include "Grid.h"
#include "Algorithm.h"

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
    Grid::Instance().Render();
    UI::Canvas::Instance().Render();
    glutSwapBuffers();

    frameCount++;

    if (glutGet(GLUT_ELAPSED_TIME) - lastTime >= 1000)
    {
        lastTime += 1000;
        frameCount = 0;
    }

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

        Grid::Instance().HandleMouseDown(glm::vec2(x, y));

        mouseX = x, mouseY = y;
        mouseDragFromX = x, mouseDragFromY = y;

        if (button == GLUT_LEFT_BUTTON)
        {
            leftPressed = true;
            /*Grid::Instance().SetStartPoint(Grid::Instance().InPoint(glm::vec2(x, y)));*/
        }
        else if (button == GLUT_MIDDLE_BUTTON)
            middlePressed = true;
        else if (button == GLUT_RIGHT_BUTTON)
            rightPressed = true;
    }
    else if (state == GLUT_UP)
    {
        if (UI::Canvas::Instance().HandleMouseUp(glm::vec2(x, y))) return;

        if (button == GLUT_LEFT_BUTTON)
            leftPressed = false;
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
        /*Grid::Instance().SetHoverPoint(Grid::Instance().InPoint(glm::vec2(x, y)));
        Grid::Instance().SetEndPoint(Grid::Instance().InPoint(glm::vec2(x, y)));*/
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
}

int main(int argc, char* argv[])
{
    glutInit(&argc, (char**)argv);

    // Initialize and create window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("2 - Stroke and Line");

    // Scaling grid background
    Grid::Instance().Scale(20);

    // Declear buttons on UI canvas
    UI::Button applyAlgorithm = UI::Button(2, SCREEN_WIDTH - 130, 30, SCREEN_WIDTH - 30, 60);
    applyAlgorithm.SetText("SCAN LINE");
    applyAlgorithm.SetOnSelectListener([]() -> void {
        Grid::Instance().ClearPoints();
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