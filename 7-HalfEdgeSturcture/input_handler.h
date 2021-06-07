#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <algorithm>

typedef void(*OnClickCallback)(glm::vec2);
typedef void(*OnDragCallback)(glm::vec2, glm::vec2);

class InputHandler final {
public:
	explicit InputHandler(GLFWwindow* window);
	void AddOnClickCallback(OnClickCallback* func);
	void AddOnDragCallback(OnDragCallback* func);
	void RemoveOnClickCallback(OnClickCallback* func);
	void RemoveOnDragCallback(OnDragCallback* func);

private:
	GLFWwindow* window;

	std::vector<OnClickCallback*> onClickCallbackList;
	std::vector<OnDragCallback*> onDragCallbackList;
};


#endif  //INPUT_HANDLER_H
