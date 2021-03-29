#include "Algorithm.h"

void Algorithm::ScanLine(class Grid* grid, class Polygon* polygon)
{
	std::vector<Polygon::Edge*> edgeList = std::vector<Polygon::Edge*>();
	EdgeLink* edgeLinkHead = new EdgeLink();
	edgeLinkHead->next = nullptr;
	edgeLinkHead->pre = nullptr;

	int scanMin = INT_MAX;
	int scanMax = INT_MIN;

	// Put edges in list; Select minimal point in vertex link
	if (polygon->GetVertexNumber() < 3) return;	// Not a polygon
	Polygon::Vertex* vertex = polygon->GetVertexHead();
	do	// Enumerate each point in polygon
	{
		// 
		scanMin = std::min(scanMin, (int)floorf(vertex->vertex.y));
		scanMax = std::max(scanMax, (int)ceilf(vertex->vertex.y));

		Polygon::Edge* newEdge;
		// The Y-coordinate of the FROM vertex should < that of the TO vertex
		if (vertex->vertex.y < vertex->next->vertex.y)
			newEdge = new Polygon::Edge{ vertex, vertex->next };
		else newEdge = new Polygon::Edge{ vertex->next, vertex };

		edgeList.push_back(newEdge);
		vertex = vertex->next;
	} while (vertex != polygon->GetVertexHead());
	
	std::sort(edgeList.begin(), edgeList.end(), EdgeYComparer);
	int edgeListCnt = 0;

	// Start line scanning
	for (int scanY = scanMin; scanY <= scanMax; scanY++)
	{
		// Insert node into edgelist
		while (edgeListCnt < (int)edgeList.size()
			&& edgeList[edgeListCnt]->from->vertex.y <= scanY)
		{
			EdgeLink* edgeLinkNode = new EdgeLink
			{
				edgeList[edgeListCnt]->from->vertex,
				edgeList[edgeListCnt]->to->vertex,
				// inverse slope
				(edgeList[edgeListCnt]->to->vertex.x - edgeList[edgeListCnt]->from->vertex.x)
					/ (edgeList[edgeListCnt]->to->vertex.y - edgeList[edgeListCnt]->from->vertex.y),
				nullptr, nullptr
			};
			InsertNodeToEdgeLink(edgeLinkHead, edgeLinkNode);
			edgeListCnt++;
		}

		// Remove out-date nodes
		RemoveNodeFromEdgeLink(edgeLinkHead);

		OneTimeSortToEdgeLink(edgeLinkHead);

		// Render row in scaning line
		EdgeLink* edgeLinkNode = edgeLinkHead->next;
		while (edgeLinkNode != nullptr)
		{
			if (edgeLinkNode->next == nullptr)
			{
				printf("[LOG]: Error: unpaired single node\n");
				return;
			}
			MarkLine(grid, scanY, (int)roundf(edgeLinkNode->now.x), (int)roundf(edgeLinkNode->next->now.x));
			edgeLinkNode = edgeLinkNode->next->next;
		}

		// Update nodes
		UpdateNodeInEdgeLink(edgeLinkHead);
	}

	if (SHOW_SCANNING_ANIMATION)
		grid->StartAnimation();
}

// Compare Y-coordinate of smaller vertex of each edge
bool Algorithm::EdgeYComparer(Polygon::Edge* a, Polygon::Edge* b)
{
	return a->from->vertex.y < b->from->vertex.y;
}

void Algorithm::InsertNodeToEdgeLink(EdgeLink* head, EdgeLink* node)
{
	EdgeLink* nowNode = head;
	while (nowNode->next != nullptr)
	{
		nowNode = nowNode->next;
		if (nowNode->now.x > node->now.x)
		{
			node->pre = nowNode->pre;
			node->next = nowNode;
			nowNode->pre->next = node;
			nowNode->pre = node;
			return;
		}
	}
	nowNode->next = node;
	node->pre = nowNode;
}

void Algorithm::OneTimeSortToEdgeLink(EdgeLink* head)
{
	EdgeLink* nowNode = head;
	while (nowNode->next != nullptr && nowNode->next->next != nullptr)
	{
		nowNode = nowNode->next;
		if (nowNode->now.x > nowNode->next->now.x)
		{
			// Swap nowNode and nowNode->next in link
			nowNode->pre->next = nowNode->next;
			if (nowNode->next->next != nullptr)
				nowNode->next->next->pre = nowNode;
			nowNode->next->pre = nowNode->pre;
			nowNode->next = nowNode->next->next;
			nowNode->pre->next->next = nowNode;
			nowNode->pre = nowNode->pre->next;
		}
	}
}

void Algorithm::UpdateNodeInEdgeLink(EdgeLink* head)
{
	EdgeLink* nowNode = head;
	while (nowNode->next != nullptr)
	{
		nowNode = nowNode->next;
		nowNode->now.x += nowNode->delta;
		nowNode->now.y++;
	}
}

void Algorithm::RemoveNodeFromEdgeLink(EdgeLink* head)
{
	EdgeLink* nowNode = head;
	EdgeLink* nextNode = nowNode->next;
	while (nextNode != nullptr)
	{
		nowNode = nextNode;
		if (nowNode->now.y >= nowNode->end.y)
		{
			// Delete nowNode
			nowNode->pre->next = nowNode->next;
			if (nowNode->next != nullptr)
				nowNode->next->pre = nowNode->pre;
			
			nextNode = nowNode->next;
			delete nowNode;
			nowNode = nullptr;
		}
		else nextNode = nowNode->next;
	}
}

void Algorithm::EdgeLinkLog(EdgeLink* head)
{
	EdgeLink* nowNode = head;
	while (nowNode->next != nullptr)
		nowNode = nowNode->next;
}

void Algorithm::MarkLine(Grid* grid, int y, int fromX, int toX)
{
	if (SHOW_SCANNING_ANIMATION)
	{
		for (int i = fromX; i <= toX; i++)
			grid->AddPointToWaitingList(i, y);
	}
	else
	{
		for (int i = fromX; i <= toX; i++)
			grid->MarkPoint(i, y);
	}
}