#include "B_Spline.h"

B_Spline::B_Spline(const Polyline* basePolyline, int degree, int precision)
{
	GenerateControlPoint(basePolyline->GetVertexNumber(), degree, ControlPointMode::UNIFORM);
	std::vector<glm::vec2> pointList;

	for (int fraction = 0; fraction <= precision; fraction++)
	{
		pointList.push_back(GetCurvePoint(basePolyline, degree, fraction * 1.0f / precision));
	}

	InitialVerticesList(pointList);
}

void B_Spline::GenerateControlPoint(int vertexNumber, int degree, ControlPointMode mode)
{
	switch (mode)
	{
	case B_Spline::ControlPointMode::UNIFORM:
		controlPointNumber = vertexNumber + degree + 1;
		controlPoint = new float[controlPointNumber];

		for (int i = 0; i < controlPointNumber; i++)
		{
			controlPoint[i] = i * 1.0f / (controlPointNumber - 1);
		}
		break;

	case B_Spline::ControlPointMode::CLAMP:
		controlPointNumber = vertexNumber + degree * 2 + 1;
		controlPoint = new float[controlPointNumber];

		for (int i = 0; i < controlPointNumber; i++)
		{
			controlPoint[i] = (i - degree + 1) * 1.0f / (controlPointNumber - degree * 2 + 1);
			if (controlPoint[i] < 0.0f)
			{
				controlPoint[i] = 0.0f;
			}
			if (controlPoint[i] > 1.0f)
			{
				controlPoint[i] = 1.0f;
			}
		}
		break;
	default:
		break;
	}

	/*printf("Control Points: ");
	for (int i = 0; i < controlPointNumber; i++)
	{
		printf("%.2f ", controlPoint[i]);
	}
	printf("\n");*/
}

glm::vec2 B_Spline::GetCurvePoint(const Polyline* basePolyline, int degree, float t)
{
	glm::vec2 res = glm::vec2(0.0f);
	// printf("t = %.3f: ", t);
	
	int index;
	Vertex* vertex;
	for (index = 0, vertex = basePolyline->GetVertexHead();
		vertex != nullptr;
		index++, vertex = vertex->next)
	{
		// printf("%.2f*[%d] ", GetBasis(index, degree, t), index);
		res += GetBasis(index, degree, t) * vertex->vertex;
	}

	// printf("\n");

	return res;
}

float B_Spline::GetBasis(int index, int degree, float t)
{
	//printf("Basis: index=%d, degree=%d, t=%.2f\n", index, degree, t);
	
	float res = 0.0f;
	// degree at first index, index at second index
	float** basis = new float* [degree + 1];

	/*printf("        ");
	for (int i = 0; i <= degree; i++)
	{
		printf("i =%2d ", i + index);
	}
	printf("\n");*/

	for (int d = 0; d <= degree; d++)
	{
		// printf("d = [%d] ", d);
		basis[d] = new float[degree - d + 1];
		for (int i = 0; i <= degree - d; i++)
		{
			if (controlPoint[index + i] <= t && t < controlPoint[index + i + d + 1])
			{
				if (d == 0)
				{
					basis[d][i] = 1;
				}
				else
				{
					basis[d][i] = ((t - controlPoint[index + i]) / (controlPoint[index + i + d] - controlPoint[index + i])) * basis[d - 1][i]
						+ ((controlPoint[index + i + d + 1] - t) / (controlPoint[index + i + d + 1] - controlPoint[index + i + 1])) * basis[d - 1][i + 1];
				}
			}
			else
			{
				basis[d][i] = 0;
			}
		}

		/*for (int i = 0; i <= degree - d; i++)
		{
			printf("%4.3f ", basis[d][i]);
		}
		printf("\n");*/
	}
	res = basis[degree][0];

	for (int d = 0; d <= degree; d++)
	{
		delete[] basis[d];
	}
	delete[] basis;

	return res;
}