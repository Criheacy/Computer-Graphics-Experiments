#pragma once

#include "Polygon.h"
#include "Settings.h"
#include "Algorithm.h"

// Singleton class
class Grid
{
public:
	static Grid& Instance();

	// DO NOT implement this class
	Grid(Grid const&) = delete;
	void operator=(Grid const&) = delete;

	void Translate(glm::vec2 deltaPosition);
	void Scale(float scale);
	void ScaleAt(float scale, glm::vec2 scalingPoint);
	void Reanchor(glm::vec2 anchor);

	// Provide interactive interface
	void HandleMouseDown(glm::vec2 position);
	void HandleMouseDrag(glm::vec2 deltaPosition);
	void HandleMouseMove(glm::vec2 deltaPosition);

	// Convert screen coordinate to point data in pointlist
	glm::vec2 InPoint(glm::vec2 src);

	glm::vec2 Transform(glm::vec2 src);
	glm::vec2 InverseTransform(glm::vec2 src);
	glm::vec2 ProjectToScreen(glm::vec2 src);

	// Draw line pixels
	void MarkPoint(int x, int y);
	void ClearPoints();

	// Render all elements
	void Render();

protected:
	Grid();

private:
	int ConstrainScale();

	// Render marked points
	void RenderPoints();

	// Render background grids line
	void RenderGridLines();

	// Render auxiliary line
	void RenderPolygon();

private:
	glm::mat3x3 transform;
	glm::mat3x3 project;
	glm::vec2 anchor;

	Polygon polygon;
};