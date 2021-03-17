#pragma once

#include "Polygon.h"
#include "Settings.h"
#include "Algorithm.h"
#include "Animator.hpp"

#define VERTEX_SELECT_DIST 1
#define EDGE_SELECT_DIST 0.8

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
	void HandleLeftMouseDown(glm::vec2 position);
	void HandleRightMouseDown(glm::vec2 position);
	void HandleMouseUp(glm::vec2 position);
	void HandleMouseDrag(glm::vec2 position);
	void HandleMouseMove(glm::vec2 position);

	void ScanLineOnPolygon();

	// Convert screen coordinate to point data in pointlist
	glm::vec2 InPoint(glm::vec2 src);

	glm::vec2 Transform(glm::vec2 src);
	glm::vec2 InverseTransform(glm::vec2 src);
	glm::vec2 ProjectToScreen(glm::vec2 src);

	// Set Animation
	void AddPointToWaitingList(int x, int y);
	void StartAnimation();
	void UpdateAnimation();

	// Draw line pixels
	void SetScanLineShownState(bool state);
	void SetScanLinePosition(glm::vec2 position);
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

	void RenderScanningLine();

	// Render auxiliary line
	void RenderPolygon();

	void RenderHoveringEdge();
	void RenderHoveringVertex();

	// Render gizmos
	void RenderCircleGizmo(glm::vec2 position, glm::vec3 color, float radius, bool filled);

private:
	glm::mat3x3 transform;
	glm::mat3x3 project;
	glm::vec2 anchor;

	Polygon polygon;

	Polygon::Vertex* hoveringVertex;
	Polygon::Edge* hoveringEdge;

	Polygon::Vertex* holdingVertex;

	Animator<int> animator = Animator<int>(0);

	bool showScanningLine;
	glm::vec2 scanningLine;	// usually render Y-coordinate
	std::vector<glm::vec2> pointList;

	std::queue<glm::vec2> waitingList;
};