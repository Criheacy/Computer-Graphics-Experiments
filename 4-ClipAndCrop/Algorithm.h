#pragma once

#include "Grid.h"
#include "Settings.h"

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

	static Polygon* CropPolygon(const class Polygon* polygon, const class Rect& clipFrame);

protected:
	static Polygon* CropPolygonByLine(class Polygon* polygon, const glm::vec2 clipLineFrom, const glm::vec2 clipLineTo);
};