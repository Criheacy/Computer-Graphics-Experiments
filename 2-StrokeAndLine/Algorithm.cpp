#include "Algorithm.h"

void Algorithm::DDALine(class Grid* grid, int fromX, int toX, int fromY, int toY)
{
	int section = ConvertToSection(fromX, toX, fromY, toY);

	float deltaX = toX - fromX;
	float deltaY = toY - fromY;
	float k = deltaY / deltaX;
	float y = fromY;

	for (int x = fromX; x <= toX; x++)
	{
		MarkMappedPoint(grid, x, (int)y + 0.5f, fromX, fromY, section);
		y = y + k;
	}
}

void Algorithm::BresenhamLine(class Grid* grid, int fromX, int toX, int fromY, int toY)
{
	//printf("From(%d,%d)To(%d,%d) --> ", fromX, fromY, toX, toY);
	int section = ConvertToSection(fromX, toX, fromY, toY);
	//printf("From(%d,%d)To(%d,%d)\n", fromX, fromY, toX, toY);

	int x = fromX;
	int y = fromY;
	int deltaX = toX - fromX;
	int deltaY = toY - fromY;

	if (deltaX >= deltaY)
	{
		int e = -deltaX;
		for (int i = 0; i <= deltaX; i++)
		{
			MarkMappedPoint(grid, x, y, fromX, fromY, section);
			x++;
			e += 2 * deltaY;
			if (e >= 0)
			{
				y++;
				e -= 2 * deltaX;
			}
		}
	}
}

int Algorithm::ConvertToSection(int& fromX, int& toX, int& fromY, int& toY)
{
	/* section:
	 * 
	 ******* deltaY ******
	 *   \     ^     /   *
	 *    \  7 | 1  /    *
	 *     \   |   /     *
	 *   6  \  |  /  0   *
	 *---------+-------> deltaX
	 *   4  /  |  \  2   *
	 *     /   |   \     *
	 *    /  5 | 3  \    *
	 *   /     |     \   *
	 ********************/

	int section = 0x0;

	int deltaX = toX - fromX;
	int deltaY = toY - fromY;

	// To.X < From.X
	if (deltaX < 0)
	{
		section |= (1 << 2);
		deltaX = -deltaX;
		deltaY = -deltaY;
	}

	// Different positive and negative
	if ((deltaX & NEG_INT_MIN) != (deltaY & NEG_INT_MIN))
	{
		section |= (1 << 1);
		deltaY = -deltaY;
	}

	// Angle > 45
	if (deltaY > deltaX)
	{
		section |= (1 << 0);
		std::swap(deltaX, deltaY);
	}

	toX = fromX + deltaX;
	toY = fromY + deltaY;
	
	return section;
}

void Algorithm::MarkMappedPoint(class Grid* grid, int x, int y, int fromX, int fromY, int section)
{
	int deltaX = x - fromX;
	int deltaY = y - fromY;

	if (section & (1 << 0))
	{
		std::swap(deltaX, deltaY);
	}

	if (section & (1 << 1))
	{
		deltaY = -deltaY;
	}

	if (section & (1 << 2))
	{
		deltaX = -deltaX;
		deltaY = -deltaY;
	}

	x = fromX + deltaX;
	y = fromY + deltaY;

	grid->MarkPoint(x, y);
}