#pragma once

#ifndef OFF_GRAPHICS_H
#define OFF_GRAPHICS_H
#include "graphics.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class OFFGraphics: public Graphics {
public:
	explicit OFFGraphics(const std::string& filePath);

private:
	OFFGraphics();

	static std::string LoadStringFromFile(const std::string& filePath);
	void DecodeOFF(const std::string& OFFString);
};


#endif  //OFF_GRAPHICS_H
