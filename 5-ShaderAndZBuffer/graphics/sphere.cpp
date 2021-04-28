#include "sphere.h"

Sphere::Sphere() = default;

Sphere::Sphere(float radius) {
	GenerateVertices(glm::vec3(0.0f), radius, 1);
	GenerateDefaultIndices();
}

Sphere::Sphere(glm::vec3 center, float radius, int subdivisionLevel) {
	GenerateVertices(center, radius, subdivisionLevel);
	GenerateDefaultIndices();
}

void Sphere::GenerateVertices(glm::vec3 center, float radius, int subdivisionLevel) {
	float m = (float) (sqrt(50.0 - 10.0 * sqrt(5.0)) * radius / 10.0);
	float n = (float) (sqrt(50.0 + 10.0 * sqrt(5.0)) * radius / 10.0);

	vertexArraySize = 12;
	vertexArray = new glm::vec3[vertexArraySize];
	for (int dim = 0; dim < 3; ++dim) {
		for (int i = 0; i < 4; ++i) {
			float _n = ((float)((i >> 0) & 1) - 0.5f) * 2 * n * radius;
			float _m = ((float)((i >> 1) & 1) - 0.5f) * 2 * m * radius;
			float _x, _y, _z;
			if (dim == 0) {
				_x = _n, _z = _m, _y = 0;
			} else if (dim == 1) {
				_z = _n, _y = _m, _x = 0;
			} else if (dim == 2) {
				_y = _n, _x = _m, _z = 0;
			}
			vertexArray[dim * 4 + i] = center + glm::vec3(_x, _y, _z);
		}
	}
}

void Sphere::GenerateDefaultIndices() {

	vertexIndicesSize = 20;
	vertexIndices = new glm::vec3[vertexIndicesSize];
	/*
	 * 0 6 4
	 * 1 4 6
	 * 2 5 7
	 * 3 7 5
	 * 0 8 2
	 * 0 2 9
	 * 1 3 10
	 * 1 11 3
	 * 4 10 8
	 * 5 8 10
	 * 6 9 11
	 * 7 11 9
	 * 0 4 8
	 * 0 9 6
	 * 1 6 11
	 * 1 10 4
	 * 2 8 5
	 * 2 7 9
	 * 3 5 10
	 * 3 11 7
	 */
	vertexIndices[0] = glm::vec3(0, 6, 4);
	vertexIndices[1] = glm::vec3(1, 4, 6);
	vertexIndices[2] = glm::vec3(2, 5, 7);
	vertexIndices[3] = glm::vec3(3, 7, 5);
	vertexIndices[4] = glm::vec3(0, 8, 2);
	vertexIndices[5] = glm::vec3(0, 2, 9);
	vertexIndices[6] = glm::vec3(1, 3, 10);
	vertexIndices[7] = glm::vec3(1, 11, 3);
	vertexIndices[8] = glm::vec3(4, 10, 8);
	vertexIndices[9] = glm::vec3(5, 8, 10);
	vertexIndices[10] = glm::vec3(6, 9, 11);
	vertexIndices[11] = glm::vec3(7, 11, 9);
	vertexIndices[12] = glm::vec3(0, 4, 8);
	vertexIndices[13] = glm::vec3(0, 9, 6);
	vertexIndices[14] = glm::vec3(1, 6, 11);
	vertexIndices[15] = glm::vec3(1, 10, 4);
	vertexIndices[16] = glm::vec3(2, 8, 5);
	vertexIndices[17] = glm::vec3(2, 7, 9);
	vertexIndices[18] = glm::vec3(3, 5, 10);
	vertexIndices[19] = glm::vec3(3, 11, 7);

}

void Sphere::Subdivision(int currentIteration, int maxIteration) {

}