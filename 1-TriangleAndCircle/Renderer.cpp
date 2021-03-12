#include "Renderer.h"

Renderer::Renderer()
{
	renderQueue = std::vector<Graphics*>();
}

void Renderer::AddGraphics(Graphics* graphics)
{
	renderQueue.push_back(graphics);
}

void Renderer::RemoveGraphics(Graphics* graphics)
{
	auto it = std::find(renderQueue.begin(), renderQueue.end(), graphics);
	if (it != renderQueue.end())
		renderQueue.erase(it);
}

std::vector<Graphics*>::iterator Renderer::GetGraphicsFirst()
{
    return renderQueue.begin();
}

std::vector<Graphics*>::iterator Renderer::GetGraphicsLast()
{
    return renderQueue.end();
}

void Renderer::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (auto it = renderQueue.begin(); it != renderQueue.end(); it++)
    {
        glBegin((*it)->GetRenderMode());
        std::vector<glm::vec3> vertices = (*it)->GetVertices();

        for (auto vit = vertices.begin(); vit != vertices.end(); vit++)
        {
            glVertex3f((*vit).x, (*vit).y, (*vit).z);
        }
        glEnd();
    }

    glutSwapBuffers();
}