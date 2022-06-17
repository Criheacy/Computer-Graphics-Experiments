#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "graphics.h"

class Cube : public Graphics
{
public:
	// Initial with given edge length and origin center point
	explicit Cube(float length);

	// Initial with given edge length and given center point
	Cube(glm::vec3 centerPoint, float xLength, float yLength, float zLength);

private:
	Cube();
	std::vector<glm::vec3>* GenerateVertices(glm::vec3 centerPoint, float xLength, float yLength, float zLength);
	std::vector<std::vector<int>>* GenerateDefaultIndices();

};


#endif  // CUBE_H
