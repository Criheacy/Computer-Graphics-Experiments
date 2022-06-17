#pragma once
#ifndef GLFW_TOOLKIT_H
#define GLFW_TOOLKIT_H

#include "input_handler.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;
const char GLFW_WINDOW_TITLE[] = "Half Edge Structure";

class GLFW {
public:
	GLFW(const GLFW&) = delete;
	void operator=(const GLFW&) = delete;
	static GLFW& GetInstance();

	bool Initialize();
	void BindVertexArray(unsigned int vertexArraySize, float* vertexArray);
	void BindIndexArray(unsigned int indexArraySize, int* indexArray);
	void Render();
	void Terminate();

	GLFWwindow* GetWindow();
	class InputHandler* GetInputHandler();

private:
	GLFW();

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double positionX, double positionY);

	unsigned int VBO, VAO, EBO;
	unsigned int vertexCount, indexCount;
	GLFWwindow* mWindow;
	class InputHandler* mInputHandler;
};


#endif  //GLFW_TOOLKIT_H
