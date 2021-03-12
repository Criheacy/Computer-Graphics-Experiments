#include <glut.h>

#include "Settings.h"
#include "Grid.h"

int mouseX;
int mouseY;
bool leftPressed;
bool middlePressed;
bool rightPressed;
int mouseDragFromX;
int mouseDragFromY;

void RenderScene(void)
{
    Grid::Instance().Render();
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
        mouseX = x, mouseY = y;
        mouseDragFromX = x, mouseDragFromY = y;
        if (button == GLUT_LEFT_BUTTON)
            leftPressed = true;
        else if (button == GLUT_MIDDLE_BUTTON)
            middlePressed = true;
        else if (button == GLUT_RIGHT_BUTTON)
            rightPressed = true;
    }
    else if (state == GLUT_UP)
    {
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
    if (leftPressed)
    {
        Grid::Instance().SetHoverPoint(Grid::Instance().InPoint(glm::vec2(x, y)));
        glm::vec2 coord = Grid::Instance().InPoint(glm::vec2(x, y));
        printf("[%d, %d]\n", (int)coord.x, (int)coord.y);
        /*float angle = (x - mouseX + y - mouseY) * 0.5f;
        mouseHolding->Rotate(angle);
        mouseX = x, mouseY = y;*/
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
    Grid::Instance().SetHoverPoint(Grid::Instance().InPoint(glm::vec2(x, y)));
}

int main(int argc, char* argv[])
{
    glutInit(&argc, (char**)argv);

    // Initialize and create window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("1 - Triangle and Circle");

    Grid::Instance().Scale(30);
    //Grid::Instance().MarkPoint(1, 1);

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