#pragma once

#include "Polyline.h"
#include "BezierCurve.h"
#include "B_Spline.h"
#include "Settings.h"
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
	void Reanchor(glm::vec2 anchor);

	// Provide interactive interface
	void HandleLeftMouseDown(glm::vec2 position);
	void HandleRightMouseDown(glm::vec2 position);
	void HandleMouseUp(glm::vec2 position);
	void HandleMouseDrag(glm::vec2 position);
	void HandleMouseMove(glm::vec2 position);
	void HandleButtonEvent();

	glm::vec2 Transform(glm::vec2 src);
	glm::vec2 InverseTransform(glm::vec2 src);
	glm::vec2 ProjectToScreen(glm::vec2 src);

	// Render all elements
	void Render();

protected:
	Grid();

private:
	void OnMouseEventUpdate();

	void RenderPolyline(const Polyline* polyline, bool closed = false);
	void RenderLine(const Polyline* polyline, float width, glm::vec3 color, bool closed = false);

	void RenderVerticalLine(int lineWidth, glm::vec2 fromPos, glm::vec2 toPos);
	void RenderHorizontalLine(int lineWidth, glm::vec2 fromPos, glm::vec2 toPos);

	void RenderHoveringEdge();
	void RenderHoveringVertex();

	// Render gizmos
	void RenderCircleGizmo(glm::vec2 position, glm::vec3 color, float radius, bool filled);

private:
	glm::mat3x3 transform;
	glm::mat3x3 project;
	glm::vec2 anchor;

	Polyline* polyline;
	Polyline::Vertex* hoveringVertex;
	Polyline::Edge* hoveringEdge;
	Polyline::Vertex* holdingVertex;

	Polyline* curve;
};