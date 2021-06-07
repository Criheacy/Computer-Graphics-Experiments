#include "glfw_toolkit.h"

GLFW::GLFW() {

}

GLFW &GLFW::GetInstance() {
	static GLFW instance;
	return instance;
}

bool GLFW::Initialize() {
	// initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 8);

	// glfw window creation
	mWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
	                           GLFW_WINDOW_TITLE,
	                                      nullptr,
	                                      nullptr);
	if (mWindow == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	return true;
}

GLFWwindow *GLFW::GetWindow() {
	return mWindow;
}

void GLFW::BindVertexArray(unsigned int vertexArraySize, float *vertexArray) {
	this->vertexCount = vertexArraySize;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first,
	// then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertexArraySize * sizeof(float)), vertexArray, GL_STATIC_DRAW);
}

void GLFW::BindIndexArray(unsigned int indexArraySize, int *indexArray) {
	this->indexCount = indexArraySize;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indexArraySize * sizeof(int)), indexArray, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
}

void GLFW::Render() {
	// render
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glfwSwapBuffers(mWindow);
}

void GLFW::Terminate() {
	// delete data array and buffers, clear all previous allocated resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}
