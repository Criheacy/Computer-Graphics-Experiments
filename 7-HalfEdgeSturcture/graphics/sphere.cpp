#include "sphere.h"

Sphere::Sphere() = default;
Sphere::Sphere(float radius) {
	auto vertexArray = GenerateVertices(glm::vec3(0.0f), radius, 1);
	auto indexArray = GenerateDefaultIndices();
	SetGraphicsArray(*vertexArray, *indexArray);

	for (int i = 0; i < SUBDIVISION_LEVEL; i++) {
		SubdivideFaces();
	}
	NormalizeVertices(radius);
}

Sphere::Sphere(glm::vec3 center, float radius, int subdivisionLevel) {
	auto vertexArray = GenerateVertices(center, radius, subdivisionLevel);
	auto indexArray = GenerateDefaultIndices();
	SetGraphicsArray(*vertexArray, *indexArray);

	for (int i = 0; i < SUBDIVISION_LEVEL; i++) {
		SubdivideFaces();
	}
	NormalizeVertices(radius);
}

std::vector<glm::vec3>* Sphere::GenerateVertices(glm::vec3 center, float radius, int subdivisionLevel) {
	vertexCount = 12;
	float m = (float) (sqrt(50.0 - 10.0 * sqrt(5.0)) * radius / 10.0);
	float n = (float) (sqrt(50.0 + 10.0 * sqrt(5.0)) * radius / 10.0);

	auto* vertexArray = new std::vector<glm::vec3>();
	vertexArray->resize(12);
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
			(*vertexArray)[dim * 4 + i] = center + glm::vec3(_x, _y, _z);
		}
	}
	return vertexArray;
}

std::vector<std::vector<int>>* Sphere::GenerateDefaultIndices() {
	edgeCount = 30;
	faceCount = 20;
	auto* indexArray = new std::vector<std::vector<int>>();
	indexArray->resize(20);
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
	(*indexArray)[0] = std::vector<int>({0, 6, 4});
	(*indexArray)[1] = std::vector<int>({1, 4, 6});
	(*indexArray)[2] = std::vector<int>({2, 5, 7});
	(*indexArray)[3] = std::vector<int>({3, 7, 5});
	(*indexArray)[4] = std::vector<int>({0, 8, 2});
	(*indexArray)[5] = std::vector<int>({0, 2, 9});
	(*indexArray)[6] = std::vector<int>({1, 3, 10});
	(*indexArray)[7] = std::vector<int>({1, 11, 3});
	(*indexArray)[8] = std::vector<int>({4, 10, 8});
	(*indexArray)[9] = std::vector<int>({5, 8, 10});
	(*indexArray)[10] = std::vector<int>({6, 9, 11});
	(*indexArray)[11] = std::vector<int>({7, 11, 9});
	(*indexArray)[12] = std::vector<int>({0, 4, 8});
	(*indexArray)[13] = std::vector<int>({0, 9, 6});
	(*indexArray)[14] = std::vector<int>({1, 6, 11});
	(*indexArray)[15] = std::vector<int>({1, 10, 4});
	(*indexArray)[16] = std::vector<int>({2, 8, 5});
	(*indexArray)[17] = std::vector<int>({2, 7, 9});
	(*indexArray)[18] = std::vector<int>({3, 5, 10});
	(*indexArray)[19] = std::vector<int>({3, 11, 7});
	return indexArray;
}

void Sphere::NormalizeVertices(float radius) {

}