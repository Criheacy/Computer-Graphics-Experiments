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

	void AddGraphics(Graphics* graphics);
	void RemoveGraphics(Graphics* graphics);
	std::vector<Graphics*>::iterator GetGraphicsFirst();
	std::vector<Graphics*>::iterator GetGraphicsLast();

	void Render();

protected:
	std::vector<Graphics*> renderQueue;
};