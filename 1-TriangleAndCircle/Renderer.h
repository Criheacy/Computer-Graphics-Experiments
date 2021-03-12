#pragma once

#include "Settings.h"
#include "Graphics.h"

#include <glut.h>
#include <vector>
#include <algorithm>

class Renderer
{
public:
	Renderer();

	// Add graphics object to render queue
	void AddGraphics(Graphics* graphics);

	// Remove graphics object from render queue; do nothing if doesn't found
	void RemoveGraphics(Graphics* graphics);

	// Iterators to provide access to render queue
	std::vector<Graphics*>::iterator GetGraphicsFirst();
	std::vector<Graphics*>::iterator GetGraphicsLast();

	void Render();

protected:
	std::vector<Graphics*> renderQueue;
};