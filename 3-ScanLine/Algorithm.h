#pragma once

#include "Grid.h"
class Algorithm
{
public:
	struct EdgeLink
	{
		glm::vec2 now;
		glm::vec2 end;
		float delta;
		EdgeLink* next;
		EdgeLink* pre;
	};

	Algorithm() = delete;
	Algorithm(const Algorithm&) = delete;
	Algorithm& operator=(const Algorithm&) = delete;

	static void ScanLine(class Grid* grid, class Polygon* polygon);

protected:
	static bool EdgeYComparer(Polygon::Edge* a, Polygon::Edge* b);

	static void InsertNodeToEdgeLink(EdgeLink* head, EdgeLink* node);

	static void OneTimeSortToEdgeLink(EdgeLink* head);

	static void UpdateNodeInEdgeLink(EdgeLink* head);

	static void RemoveNodeFromEdgeLink(EdgeLink* head);

	static void EdgeLinkLog(EdgeLink* head);

	static void MarkLine(Grid* grid, int y, int fromX, int toX);
};