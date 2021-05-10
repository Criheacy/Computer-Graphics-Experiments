#pragma once
#include "Polyline.h"
class B_Spline : public Polyline
{
public:
	enum class ControlPointMode
	{
		UNIFORM,
		CLAMP,
	};

	B_Spline(const Polyline* basePolyline, int degree, int precision);

	void GenerateControlPoint(int vertexNumber, int degree,
		ControlPointMode mode = ControlPointMode::UNIFORM);

	glm::vec2 GetCurvePoint(const Polyline* basePolyline, int degree, float t);
	float GetBasis(int index, int degree, float t);

private:
	float* controlPoint;
	int controlPointNumber;


};

