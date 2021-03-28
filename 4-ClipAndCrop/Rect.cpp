#include "Rect.h"

Rect::Rect()
{
	InitialRect(0, 0, SCREEN_WIDTH * 0.6, SCREEN_HEIGHT * 0.4);
}

void Rect::InitialRect(float centerX, float centerY, float width, float height)
{
	topY = centerY - height / 2;
	leftX = centerX - width / 2;
	bottomY = centerY + height / 2;
	rightX = centerX + width / 2;
}

void Rect::MoveTo(RD dir, glm::vec2 dist)
{
	if (dir & TOP) topY = dist.y;
	if (dir & LEFT) leftX = dist.x;
	if (dir & BOTTOM) bottomY = dist.y;
	if (dir & RIGHT) rightX = dist.x;
	if (topY > bottomY) std::swap(topY, bottomY);
	if (leftX > rightX) std::swap(leftX, rightX);
}

glm::vec2 Rect::GetDir(RD dir) const
{
	glm::vec2* result = new glm::vec2();
	if (dir & TOP) result->y = topY;
	if (dir & LEFT) result->x = leftX;
	if (dir & BOTTOM) result->y = bottomY;
	if (dir & RIGHT) result->x = rightX;
	return *result;
}

RD Rect::GetNearestEdge(glm::vec2 point)
{
	RD result = 0;
	float minDist = INT_MAX;
	if (fabs(point.y - topY) < minDist)
	{
		minDist = fabs(point.y - topY);
		result = TOP;
	}
	if (fabs(point.x - leftX) < minDist)
	{
		minDist = fabs(point.x - leftX);
		result = LEFT;
	}
	if (fabs(point.y - bottomY) < minDist)
	{
		minDist = fabs(point.y - bottomY);
		result = BOTTOM;
	}
	if (fabs(point.x - rightX) < minDist)
	{
		minDist = fabs(point.x - rightX);
		result = RIGHT;
	}
	return result;
}

float Rect::GetDistanceToEdge(glm::vec2 point, RD edge)
{
	if (edge & TOP) return fabs(point.y - topY);
	if (edge & LEFT) return fabs(point.x - leftX);
	if (edge & BOTTOM) return fabs(point.y - bottomY);
	if (edge & RIGHT) return fabs(point.x - rightX);
	return 0;
}

RD Rect::GetNearestVertex(glm::vec2 point)
{
	RD result = 0;

	if (fabs(point.y - topY) < fabs(point.y - bottomY)) result |= TOP;
	else result |= BOTTOM;

	if (fabs(point.x - leftX) < fabs(point.x - rightX)) result |= LEFT;
	else result |= RIGHT;

	return result;
}

float Rect::GetDistanceToVertex(glm::vec2 point, RD vertex)
{
	glm::vec2 result = glm::vec2();
	if (vertex & TOP) result.y = fabs(point.y - topY);
	if (vertex & LEFT) result.x = fabs(point.x - leftX);
	if (vertex & BOTTOM) result.y = fabs(point.y - bottomY);
	if (vertex & RIGHT) result.x = fabs(point.x - rightX);
	return glm::length(result);
}