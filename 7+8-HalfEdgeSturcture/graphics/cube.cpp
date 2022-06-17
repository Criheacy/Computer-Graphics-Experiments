#include "cube.h"

Cube::Cube() = default;

Cube::Cube(float length) : Graphics()
{
	auto vertexArray = GenerateVertices(glm::vec3(), length, length, length);
	auto indexArray = GenerateDefaultIndices();
	SetGraphicsArray(*vertexArray, *indexArray);
}

Cube::Cube(glm::vec3 centerPoint, float xLength, float yLength, float zLength)
{
	auto vertexArray = GenerateVertices(centerPoint, xLength, yLength, zLength);
	auto indexArray = GenerateDefaultIndices();
	SetGraphicsArray(*vertexArray, *indexArray);
}

std::vector<glm::vec3>* Cube::GenerateVertices(glm::vec3 centerPoint, float xLength, float yLength, float zLength)
{
	vertexCount = 8;
	auto vertexArray = new std::vector<glm::vec3>();
	vertexArray->resize(8);

	for (int i=0; i<8; i++)
	{
		// Set vertex to the center vector +/- corresponding length
		// Use bit comparison when generating vertex coordinate
		/*
		 *      [6]-------[7]
		 *      /|        /|
		 *     / |       / |
		 *   [4]-------[5] |
		 *    | [2]---- |-[3]
		 *    | /       | /
		 *    |/        |/
		 *   [0]-------[1]
		 */
		(*vertexArray)[i] = glm::vec3(centerPoint.x + ((float)((i >> 0) & 1) - 0.5f) * xLength,
		                              centerPoint.y + ((float)((i >> 1) & 1) - 0.5f) * yLength,
		                              centerPoint.z + ((float)((i >> 2) & 1) - 0.5f) * zLength);
	}
	return vertexArray;
}

std::vector<std::vector<int>>* Cube::GenerateDefaultIndices()
{
	edgeCount = 18;
	faceCount = 12;
	auto indexArray = new std::vector<std::vector<int>>();
	indexArray->resize(12);

	// the default cube indices
	// orders are followed by right-hand system
	/*
	 * 0 3 1
	 * 0 2 3
	 * 0 1 5
	 * 0 5 4
	 * 0 6 2
	 * 0 4 6
	 * 7 1 3
	 * 7 5 1
	 * 7 3 2
	 * 7 2 6
	 * 7 4 5
	 * 7 6 4
	 */
	(*indexArray)[0] = {0, 3, 1};
	(*indexArray)[1] = {0, 2, 3};
	(*indexArray)[2] = {0, 1, 5};
	(*indexArray)[3] = {0, 5, 4};
	(*indexArray)[4] = {0, 6, 2};
	(*indexArray)[5] = {0, 4, 6};
	(*indexArray)[6] = {7, 1, 3};
	(*indexArray)[7] = {7, 5, 1};
	(*indexArray)[8] = {7, 3, 2};
	(*indexArray)[9] = {7, 2, 6};
	(*indexArray)[10] = {7, 4, 5};
	(*indexArray)[11] = {7, 6, 4};
	return indexArray;
}
