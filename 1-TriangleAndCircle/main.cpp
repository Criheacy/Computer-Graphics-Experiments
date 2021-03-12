#include <glut.h>

#include "Settings.h"
#include "Graphics.h"
#include "Renderer.h"

int mouseX;
int mouseY;
bool leftPressed;
bool rightPressed;
int mouseDragFromX;
int mouseDragFromY;

Renderer* renderer;
Graphics* mouseHolding;

void RenderScene(void)
{
    renderer->Render();
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
        printf("[%d %d]\n", x, y);
        for (auto graphic = renderer->GetGraphicsFirst();
            graphic != renderer->GetGraphicsLast(); graphic++)
            if ((*graphic)->InObject(glm::vec2(x, y)))
            {
                mouseHolding = *graphic;
                printf("FLAG\n");
                mouseX = mouseDragFromX = x;
                mouseY = mouseDragFromY = y;
            }

        if (button == GLUT_LEFT_BUTTON)
            leftPressed = true;
        else if (button == GLUT_RIGHT_BUTTON)
            rightPressed = true;
    }
    else if (state == GLUT_UP)
    {
        mouseHolding = NULL;
        if (button == GLUT_LEFT_BUTTON)
            leftPressed = false;
        else if (button == GLUT_RIGHT_BUTTON)
            rightPressed = false;
    }
}

void HandleMouseMotionEvent(int x, int y)
{
    if (mouseHolding != NULL)
    {
        if (leftPressed)
        {
            float angle = sqrtf((mouseDragFromX - x) * (mouseDragFromX - x)
                + (mouseDragFromY - y) * (mouseDragFromY - y));
            mouseHolding->Rotate(angle * 0.1f);
        }
        if (rightPressed)
        {
            glm::vec2 delta = glm::vec2(x - mouseX, y - mouseY);
            mouseHolding->Translate(delta);
            mouseX = x, mouseY = y;
        }
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, (char**)argv);

    // Initialize and create window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("1 - Triangle and Circle");

    // Instantiate graphics and renderer
    renderer = new Renderer();
    Triangle* triangle = new Triangle(250);
    Circle* circle = new Circle(100);

    triangle->Translate(glm::vec2(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.5f));
    circle->Translate(glm::vec2(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.5f));

    renderer->AddGraphics((Graphics*)(triangle));
    renderer->AddGraphics((Graphics*)(circle));

    // Add callback functions
    glutReshapeFunc(Resize);
    glutDisplayFunc(RenderScene);
    glutMouseFunc(HandleMouseButtonEvent);
    glutMotionFunc(HandleMouseMotionEvent);

    // Enter main loop
    glutMainLoop();
    return 0;
}