#include "input_handler.h"

void InputHandler::AddOnClickCallback(OnClickCallback *func) {
	onClickCallbackList.push_back(func);
}

void InputHandler::AddOnDragCallback(OnDragCallback *func) {
	onDragCallbackList.push_back(func);
}

InputHandler::InputHandler(GLFWwindow *window) {
	this->window = window;
}

void InputHandler::RemoveOnClickCallback(OnClickCallback *func) {
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
}
