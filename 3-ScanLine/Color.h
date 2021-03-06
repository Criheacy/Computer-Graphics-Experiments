#pragma once
#include "Settings.h"

class Color
{
public:
	static unsigned int GetGradientNumber();
	static std::pair<glm::vec3, glm::vec3> GetGradient(int number);
	static glm::vec3 GetGradientColor(int number, float ratio);

private:
	static constexpr std::pair<glm::vec3, glm::vec3> gradients[12] = {
		std::make_pair(glm::vec3(0xF9, 0x77, 0x94), glm::vec3(0x62, 0x3A, 0xA2)),
		std::make_pair(glm::vec3(0xFC, 0xCF, 0x31), glm::vec3(0xF5, 0x55, 0x55)),
		std::make_pair(glm::vec3(0x43, 0xCB, 0xFF), glm::vec3(0x97, 0x08, 0xCC)),
		std::make_pair(glm::vec3(0x92, 0xFF, 0xC0), glm::vec3(0x00, 0x26, 0x61)),
		std::make_pair(glm::vec3(0x60, 0x18, 0xDC), glm::vec3(0xD9, 0x39, 0xCD)),
		std::make_pair(glm::vec3(0xEE, 0xCE, 0x13), glm::vec3(0xB2, 0x10, 0xFF)),
		std::make_pair(glm::vec3(0x97, 0xAB, 0xFF), glm::vec3(0x12, 0x35, 0x97)),
		std::make_pair(glm::vec3(0xFD, 0x65, 0x85), glm::vec3(0x0D, 0x25, 0xB9)),
		std::make_pair(glm::vec3(0x65, 0xFD, 0xF0), glm::vec3(0x1D, 0x6F, 0xA3)),
		std::make_pair(glm::vec3(0x3B, 0x26, 0x67), glm::vec3(0xBC, 0x78, 0xEC)),
		std::make_pair(glm::vec3(0x81, 0xFF, 0xEF), glm::vec3(0xF0, 0x67, 0xB4)),
		std::make_pair(glm::vec3(0xFF, 0xA8, 0xA8), glm::vec3(0xFC, 0xFF, 0x00)),
	};
};