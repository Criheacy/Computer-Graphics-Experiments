#pragma once
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "glfw_toolkit.h"

#include <vector>
#include <algorithm>

typedef void(*OnClickCallback)(int button, glm::vec2 mousePos);
typedef void(*OnDragCallback)(int button, glm::vec2 startPos, glm::vec2 endPos);

class InputHandler final {
public:
	explicit InputHandler(GLFWwindow* window);
	void AddOnClickCallback(OnClickCallback func);
	void AddOnDragCallback(OnDragCallback func);
	// void RemoveOnClickCallback(OnClickCallback* func);
	// void RemoveOnDragCallback(OnDragCallback* func);

private:
	friend class GLFW;
	GLFWwindow* window;

	struct MouseButtonState {
		glm::vec2 lastPressPosition;
		glm::vec2 lastReleasePosition;

		bool isDragging;
		bool isClicking;
	};
	MouseButtonState mouseStateList[10];
	glm::vec2 lastFramePosition;

	void HandleMouseButtonInput(int button, int action, int mods);
	void HandleMouseMotion(glm::vec2 position);

	std::vector<OnClickCallback> onClickCallbackList;
	std::vector<OnDragCallback> onDragCallbackList;
};


#endif  //INPUT_HANDLER_H
