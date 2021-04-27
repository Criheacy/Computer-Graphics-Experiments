#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "../graphics.h"

class Cube : public Graphics
{
public:
	// Initial with given edge length and origin center point
	explicit Cube(float length);

	// Initial with given edge length and given center point
	Cube(glm::vec3 centerPoint, float xLength, float yLength, float zLength);

private:
	Cube();
	void SetCubeVertices(glm::vec3 centerPoint, float xLength, float yLength, float zLength);
	void GenerateDefaultIndices();

};


#endif  // CUBE_H
