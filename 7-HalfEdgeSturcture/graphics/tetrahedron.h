#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "graphics.h"

class Tetrahedron : public Graphics
{
public:
	explicit Tetrahedron(float length);
	Tetrahedron(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);


private:
	Tetrahedron();
	std::vector<glm::vec3>* GenerateVertices(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);
	std::vector<std::vector<int>>* GenerateDefaultIndices();
};


#endif  //TETRAHEDRON_H
