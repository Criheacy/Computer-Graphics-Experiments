#pragma once

#include "Grid.h"
class Algorithm
{
public:
	static void DDALine(class Grid* grid, int fromX, int toX, int fromY, int toY);

	static void BresenhamLine(class Grid* grid, int fromX, int toX, int fromY, int toY);

protected:
	static void MarkMappedPoint(class Grid* grid, int x, int y, int fromX, int fromY, int section);

	static int ConvertToSection(int& fromX, int& toX, int& fromY, int& toY);
};