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
