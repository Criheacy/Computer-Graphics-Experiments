#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "../graphics.h"
class Sphere : public Graphics {
public:
	explicit Sphere(float radius);
	Sphere(glm::vec3 center, float radius, int subdivisionLevel = -1);

private:
	Sphere();

	void GenerateVertices(glm::vec3 center, float radius, int subdivisionLevel);
	void GenerateDefaultIndices();
	void Subdivision(int currentIteration, int maxIteration);
	static const int SUBDIVISION_LEVEL = 1;
};


#endif  //SPHERE_H
