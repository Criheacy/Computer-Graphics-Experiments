#pragma once
#include "Polyline.h"
class BezierCurve : public Polyline
{
public:
	BezierCurve(const Polyline* basePolyline, int precision);
	glm::vec2 GetCurvePoint(const Polyline* polyline, float t);

private:
	void AscendCurveOrder(Polyline* polyline, float t);
	glm::vec2 Interpolate(glm::vec2 from, glm::vec2 to, float t);
};

