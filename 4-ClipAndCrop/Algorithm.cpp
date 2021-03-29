#include "Algorithm.h"

class Polygon* Algorithm::CropPolygon(const class Polygon* polygon, const class Rect* clipFrame)
{
	Polygon* result = new Polygon(*polygon);

	Polygon* temp = result;
	result = CropPolygonByLine(temp, clipFrame->GetDir(Rect::LEFT | Rect::TOP), clipFrame->GetDir(Rect::TOP | Rect::RIGHT));
	delete temp; temp = result;
	result = CropPolygonByLine(temp, clipFrame->GetDir(Rect::TOP | Rect::RIGHT), clipFrame->GetDir(Rect::RIGHT | Rect::BOTTOM));
	delete temp; temp = result;
	result = CropPolygonByLine(temp, clipFrame->GetDir(Rect::RIGHT | Rect::BOTTOM), clipFrame->GetDir(Rect::BOTTOM | Rect::LEFT));
	delete temp; temp = result;
	result = CropPolygonByLine(temp, clipFrame->GetDir(Rect::BOTTOM | Rect::LEFT), clipFrame->GetDir(Rect::LEFT | Rect::TOP));
	delete temp; temp = nullptr;

	return result;
}

class Polygon* Algorithm::CropPolygonByLine(class Polygon* polygon, const glm::vec2 clipLineFrom, const glm::vec2 clipLineTo)
{
	glm::vec2 clipLine = clipLineTo - clipLineFrom;

	std::vector<glm::vec2> vertexList;

	Polygon::Vertex* nowVertex = polygon->GetVertexHead();
	do
	{
		if (glm::cross(glm::vec3(nowVertex->vertex - clipLineFrom, 0.0f), glm::vec3(clipLine, 0.0f)).z < 0)
			// Vertex in clipping area
			vertexList.push_back(nowVertex->vertex);

		glm::mat2x2 matrix = glm::mat2x2(nowVertex->next->vertex - nowVertex->vertex, -clipLine);
		
		if (glm::determinant(matrix) != 0)
		{
			glm::vec2 ans = glm::inverse(matrix) * glm::vec2(clipLineFrom - nowVertex->vertex);
			if (ans.x > 0 && ans.x < 1)
				vertexList.push_back(nowVertex->vertex + ans.x * (nowVertex->next->vertex - nowVertex->vertex));
		}

		nowVertex = nowVertex->next;
	} while (nowVertex != polygon->GetVertexHead());

	Polygon* result = new Polygon(vertexList);
	return result;
}