#include "offgraphic.h"

OFFGraphics::OFFGraphics() = default;

OFFGraphics::OFFGraphics(const std::string& filePath) {
	std::string OFFString = LoadStringFromFile(filePath);
	DecodeOFF(OFFString);
}

std::string OFFGraphics::LoadStringFromFile(const std::string& filePath) {
	std::string string;
	std::ifstream inputFile;
	// Make sure 'ifstream' object can raise exceptions
	inputFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		inputFile.open(filePath);
		std::stringstream fileStringStream;
		fileStringStream << inputFile.rdbuf();
		inputFile.close();
		// Convert stream to string
		string = fileStringStream.str();
	}
	catch(std::ifstream::failure &e)
	{
		std::cout << "ERROR::OFF_GRAPHICS::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	return string;
}

void OFFGraphics::DecodeOFF(std::string OFFString) {
	std::stringstream stringStream(OFFString);
	std::string word;

	stringStream >> word;   // ignore "OFF" at first line
	try {
		stringStream >> word; vertexCount = std::stoi(word);
		stringStream >> word; faceCount = std::stoi(word);
		stringStream >> word; edgeCount = std::stoi(word);
	} catch (std::invalid_argument& argument) {
		std::cout << "ERROR::OFF_GRAPHICS::DECODE_ERROR" << std::endl;
		std::cout << " - Declaration of item numbers not found." << std::endl;
	}

	auto* vertexArray = new std::vector<glm::vec3>();
	vertexArray->resize(vertexCount);

	float x, y, z;
	for (int i = 0; i < vertexCount; ++i) {
		try {
			stringStream >> word; x = std::stof(word);
			stringStream >> word; y = std::stof(word);
			stringStream >> word; z = std::stof(word);
		} catch (std::invalid_argument& argument) {
			std::cout << "ERROR::OFF_GRAPHICS::DECODE_ERROR" << std::endl;
			std::cout << " - Cannot parse certain coordinate: " << word << std::endl;
		}
		(*vertexArray)[i] = glm::vec3(x, y, z);
	}

	auto indexArray = new std::vector<std::vector<int>>();
	indexArray->resize(faceCount);

	int edgeInFaceCount;
	for (int i = 0; i < faceCount; ++i) {
		try {
			stringStream >> word;
			edgeInFaceCount = std::stoi(word);
			if (edgeInFaceCount != 3) {
				printf("WARNING: %d index size not support\n", edgeInFaceCount);
			}
			for (int j = 0; j < edgeInFaceCount; ++j) {
				stringStream >> word;
				(*indexArray)[i].push_back(std::stoi(word));
			}
		} catch (std::invalid_argument& argument) {
			std::cout << "ERROR::OFF_GRAPHICS::DECODE_ERROR" << std::endl;
			std::cout << " - Cannot parse certain index: " << word << std::endl;
		}
	}

	LogTest();
}