#include "input_handler.h"

void InputHandler::AddOnClickCallback(OnClickCallback func) {
	onClickCallbackList.push_back(func);
}

void InputHandler::AddOnDragCallback(OnDragCallback func) {
	onDragCallbackList.push_back(func);
}

InputHandler::InputHandler(GLFWwindow *window) {
	this->window = window;

	for (int i = 0; i < 10; ++i) {
		mouseStateList[i].lastPressPosition = glm::vec2(0.0f, 0.0f);
		mouseStateList[i].lastReleasePosition = glm::vec2(0.0f, 0.0f);
		mouseStateList[i].isClicking = false;
		mouseStateList[i].isDragging = false;
	}
}

/*void InputHandler::RemoveOnClickCallback(OnClickCallback *func) {
	auto it = std::find(onClickCallbackList.begin(), onClickCallbackList.end(), func);
	if (it != onClickCallbackList.end()) {
		onClickCallbackList.erase(it);
	}
}

void InputHandler::RemoveOnDragCallback(OnDragCallback *func) {
	auto it = std::find(onDragCallbackList.begin(), onDragCallbackList.end(), func);
	if (it != onDragCallbackList.end()) {
		onDragCallbackList.erase(it);
	}
}*/

void InputHandler::HandleMouseButtonInput(int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		mouseStateList[button].isClicking = true;
		mouseStateList[button].isDragging = false;
		mouseStateList[button].lastPressPosition = lastFramePosition;
	}

	if (action == GLFW_RELEASE) {
		if (mouseStateList[button].isClicking) {
			for (auto onClick : onClickCallbackList) {
				onClick(button, mouseStateList[button].lastPressPosition);
			}
		}
		mouseStateList[button].isClicking = false;
		mouseStateList[button].isDragging = false;
		mouseStateList[button].lastReleasePosition = lastFramePosition;
	}
}

void InputHandler::HandleMouseMotion(glm::vec2 position) {
	for (int i = 0; i < 10; i++) {
		if (mouseStateList[i].isClicking) {
			if (glm::distance(mouseStateList[i].lastPressPosition, position) >= 10) {
				mouseStateList[i].isClicking = false;
				mouseStateList[i].isDragging = true;
			}
		}
		if (mouseStateList[i].isDragging) {
			for (auto onDrag : onDragCallbackList) {
				onDrag(i, lastFramePosition, position);
			}
		}
	}

	lastFramePosition = position;
}
