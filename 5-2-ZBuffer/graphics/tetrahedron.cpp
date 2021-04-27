#include "tetrahedron.h"

Tetrahedron::Tetrahedron() = default;

Tetrahedron::Tetrahedron(float length)
{
	// Generate regular tetrahedron vertices:
	// Center of tetrahedron is at origin point, one of the vertices is on the z-axis,
	// others are parallel to xy-plane; one of the other is right below x-axis
	// The following are coordinates of the 4 vertices:
	// (0, 0, √6a/4)
	// (√3a/3, 0, -√6a/12)
	// (-√3a/6, -a/2, -√6a/12)
	// (-√3a/6, a/2, -√6a/12)

	float sqrt3 = sqrtf(3.0f);
	float sqrt6 = sqrtf(6.0f);
	SetTetrahedronVertices(glm::vec3(0, 0, sqrt6 * length / 4),
	                       glm::vec3(sqrt3 * length / 3, 0, -sqrt6 * length / 12),
	                       glm::vec3(-sqrt3 * length / 6, -length / 2, -sqrt6 * length / 12),
	                       glm::vec3(-sqrt3 * length / 6, length / 2, -sqrt6 * length / 12));

	GenerateDefaultIndices();
}

Tetrahedron::Tetrahedron(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4)
{
	SetTetrahedronVertices(point1, point2, point3, point4);
	GenerateDefaultIndices();
}

void Tetrahedron::SetTetrahedronVertices(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4)
{
	vertexArraySize = 4;
	vertexArray = new glm::vec3[vertexArraySize];
	/*
	 *         [0]
	 *        / | \
	 *       /  |  \
	 *      /   |   \
	 *   [3]--- |----[2]
	 *      \   |   /
	 *        \ | /
	 *         [1]
	 */
	vertexArray[0] = point1;
	vertexArray[1] = point2;
	vertexArray[2] = point3;
	vertexArray[3] = point4;
}

void Tetrahedron::GenerateDefaultIndices()
{
	vertexIndicesSize = 4;
	vertexIndices = new glm::vec3[vertexIndicesSize];

	// the default cube indices
	// orders are followed by right-hand system
	/*
	 * 0 1 3
	 * 0 2 1
	 * 0 3 2
	 * 1 2 3
	 */
	vertexIndices[0] = glm::vec3(0, 3, 1);
	vertexIndices[1] = glm::vec3(0, 1, 2);
	vertexIndices[2] = glm::vec3(0, 2, 3);
	vertexIndices[3] = glm::vec3(1, 3, 2);
}