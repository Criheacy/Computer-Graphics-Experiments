#include "Color.h"

unsigned int Color::GetGradientNumber()
{
	std::uniform_int_distribution<unsigned> uni(0, 11);
	std::default_random_engine engine;
	return uni(engine);
}

std::pair<glm::vec3, glm::vec3> Color::GetGradient(int number)
{
	return gradients[number];
}

glm::vec3 Color::GetGradientColor(int number, float ratio)
{
	return (1 - ratio) * gradients[number].first + ratio * gradients[number].second;
}