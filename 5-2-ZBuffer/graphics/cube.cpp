#include "cube.h"

Cube::Cube() = default;

Cube::Cube(float length) : Graphics()
{
	SetCubeVertices(glm::vec3(), length, length, length);
	GenerateDefaultIndices();
}

Cube::Cube(glm::vec3 centerPoint, float xLength, float yLength, float zLength)
{
	SetCubeVertices(centerPoint, xLength, yLength, zLength);
	GenerateDefaultIndices();
}

Cube& Cube::operator=(const Cube& origin)
{
	vertexArraySize = origin.vertexArraySize;
	for (int i=0; i<vertexArraySize; i++)
	{
		vertexArray[i] = origin.vertexArray[i];
	}
	vertexIndicesSize = origin.vertexIndicesSize;
	for (int i=0; i<vertexIndicesSize; i++)
	{
		vertexIndices[i] = origin.vertexIndices[i];
	}
	return *this;
}

Cube::Cube(const Cube& origin) : Graphics(origin)
{
	vertexArraySize = origin.vertexArraySize;
	for (int i=0; i<vertexArraySize; i++)
	{
		vertexArray[i] = origin.vertexArray[i];
	}
	vertexIndicesSize = origin.vertexIndicesSize;
	for (int i=0; i<vertexIndicesSize; i++)
	{
		vertexIndices[i] = origin.vertexIndices[i];
	}
}

void Cube::SetCubeVertices(glm::vec3 centerPoint, float xLength, float yLength, float zLength)
{
	vertexArraySize = 8;
	vertexArray = new glm::vec3[vertexArraySize];

	for (int i=0; i<8; i++)
	{
		// Set vertex to the center vector +/- corresponding length
		// Use bit comparison when generating vertex coordinate
		/*
		 *      [5]-------[7]
		 *      /|        /|
		 *     / |       / |
		 *   [4]-------[6] |
		 *    | [1]---- |-[3]
		 *    | /       | /
		 *    |/        |/
		 *   [0]-------[2]
		 */
		vertexArray[i] = glm::vec3(centerPoint.x + ((float)((i >> 0) & 1) - 0.5f) * xLength,
		                           centerPoint.y + ((float)((i >> 1) & 1) - 0.5f) * yLength,
		                           centerPoint.z + ((float)((i >> 2) & 1) - 0.5f) * zLength);
	}
}

void Cube::GenerateDefaultIndices()
{
	vertexIndicesSize = 12;
	vertexIndices = new glm::vec3[vertexIndicesSize];

	// the default cube indices
	// orders are followed by right-hand system
	/*
	 * 0 1 2
	 * 0 2 4
	 * 0 2 3
	 * 0 3 1
	 * 0 4 6
	 * 0 6 2
	 * 7 2 1
	 * 7 1 3
	 * 7 6 4
	 * 7 4 2
	 * 7 3 2
	 * 7 2 6
	 */
	vertexIndices[0] = glm::vec3(0, 1, 2);
	vertexIndices[1] = glm::vec3(0, 2, 4);
	vertexIndices[2] = glm::vec3(0, 2, 3);
	vertexIndices[3] = glm::vec3(0, 3, 4);
	vertexIndices[4] = glm::vec3(0, 4, 2);
	vertexIndices[5] = glm::vec3(0, 6, 4);
	vertexIndices[6] = glm::vec3(7, 2, 1);
	vertexIndices[7] = glm::vec3(7, 1, 3);
	vertexIndices[8] = glm::vec3(7, 6, 4);
	vertexIndices[9] = glm::vec3(7, 4, 2);
	vertexIndices[10] = glm::vec3(7, 3, 2);
	vertexIndices[11] = glm::vec3(7, 2, 6);
}

Cube::~Cube()
{
	// unnecessary for null pointer checking:
	// delete[] nullptr has no effect
	delete[] vertexIndices;
	delete[] vertexArray;
}
