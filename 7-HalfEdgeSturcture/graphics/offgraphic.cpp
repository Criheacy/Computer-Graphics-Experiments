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

	int vertexNumber = 0;
	int faceNumber = 0;
	int edgeNumber = 0;

	stringStream >> word;   // ignore "OFF" at first line
	try {
		stringStream >> word; vertexNumber = std::stoi(word);
		stringStream >> word; faceNumber = std::stoi(word);
		stringStream >> word; edgeNumber = std::stoi(word);
	} catch (std::invalid_argument& argument) {
		std::cout << "ERROR::OFF_GRAPHICS::DECODE_ERROR" << std::endl;
		std::cout << " - Declaration of item numbers not found." << std::endl;
	}

	vertexArraySize = vertexNumber;
	vertexArray = new glm::vec3[vertexArraySize];
	float x, y, z;
	for (int i = 0; i < vertexNumber; ++i) {
		try {
			stringStream >> word; x = std::stof(word);
			stringStream >> word; y = std::stof(word);
			stringStream >> word; z = std::stof(word);
		} catch (std::invalid_argument& argument) {
			std::cout << "ERROR::OFF_GRAPHICS::DECODE_ERROR" << std::endl;
			std::cout << " - Cannot parse certain coordinate: " << word << std::endl;
		}
		vertexArray[i] = glm::vec3(x, y, z);
	}

	vertexIndicesSize = faceNumber;
	vertexIndices = new glm::vec3[vertexIndicesSize];
	int indexListSize;
	int* indexList;
	for (int i = 0; i < vertexNumber; ++i) {
		try {
			stringStream >> word; indexListSize = std::stoi(word);
			indexList = new int[indexListSize];
			for (int j = 0; j < indexListSize; ++j) {
				stringStream >> word; indexList[j] = std::stoi(word);
			}
		} catch (std::invalid_argument& argument) {
			std::cout << "ERROR::OFF_GRAPHICS::DECODE_ERROR" << std::endl;
			std::cout << " - Cannot parse certain index: " << word << std::endl;
		}

		// only support 3-size index by now
		vertexIndices[i] = glm::vec3(indexList[0], indexList[1], indexList[2]);
	}

	LogTest();
}