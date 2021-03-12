#include "Algorithm.h"

void Algorithm::DDALine(class Grid* grid, int fromX, int toX, int fromY, int toY)
{
	float deltaX = toX - fromX;
	float deltaY = toY - fromY;
	float k = deltaY / deltaX;
	float y = fromY;

	for (int x = fromX; x <= toX; x++)
	{
		grid->MarkPoint(x, (int)y + 0.5f);
		y = y + k;
	}
}

void Algorithm::BresenhamLine(class Grid* grid, int fromX, int toX, int fromY, int toY)
{
	int x = fromX;
	int y = fromY;
	int deltaX = toX - fromX;
	int deltaY = toY - fromY;
	int e = -deltaX;

	for (int i = 0; i <= deltaX; i++)
	{
		grid->MarkPoint(x, y);
		x++;
		e += 2 * deltaY;
		if (e >= 0)
		{
			y++;
			e -= 2 * deltaX;
		}
	}
}