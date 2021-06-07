#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glfw_toolkit.h"

#include "graphics.h"
#include "graphics/cube.h"
#include "graphics/offgraphic.h"
#include "graphics/sphere.h"
#include "graphics/tetrahedron.h"
#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
	GLFW::GetInstance().Initialize();

	auto window = GLFW::GetInstance().GetWindow();

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader shader("../shader/frame.vs",
	              "../shader/frame.gs",
	              "../shader/frame.fs");

//	Shader shader("../shader/shader.vs",
//	              "../shader/shader.gs",
//	              "../shader/phong.fs");

	// Cube cube = Cube(0.8f);
	// Tetrahedron tet = Tetrahedron(0.7f);
	Sphere sphere = Sphere(0.7f);
	// OFFGraphics* offGraphics = new OFFGraphics("../res/bunny.off");

	Space::GetInstance().LogTest();


	unsigned int vertexCount = Space::GetInstance().GetSerializedVerticesArraySize();
	float *vertexArray = Space::GetInstance().GetSerializedVerticesArrayPtr();

	unsigned int indexCount = Space::GetInstance().GetSerializedIndicesArraySize();
	int *indexArray = Space::GetInstance().GetSerializedIndicesArrayPtr();

	GLFW::GetInstance().BindVertexArray(vertexCount, vertexArray);
	GLFW::GetInstance().BindIndexArray(indexCount, indexArray);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		float timeValue = (float) glfwGetTime();
		glm::mat4 view = glm::mat4(1.0);

		/// scale for bunny
		// view = glm::scale(view, glm::vec3(5.0f, 5.0f, 5.0f));

		/// scale for bumpy
		// view = glm::scale(view, glm::vec3(0.1f, 0.1f, 0.1f));
		view = glm::rotate(view, glm::radians(timeValue * 20), glm::vec3(1.0f, 1.0f, 1.0f));

		shader.Activate();
		shader.SetMat4("view", view);

		shader.SetVec4("mainColor", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		shader.SetVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader.SetVec3("lightDirection", glm::normalize(glm::vec3(0.2f, 0.1f, 1.0f)));
		shader.SetVec3("viewPosition", glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
		shader.SetFloat("screenWidth", SCREEN_WIDTH);
		shader.SetFloat("screenHeight", SCREEN_HEIGHT);

		shader.SetVec4("mainColor", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		shader.SetVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader.SetVec3("lightDirection", glm::normalize(glm::vec3(0.2f, 0.1f, 1.0f)));
		shader.SetVec3("viewPosition", glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
		shader.SetFloat("screenWidth", SCREEN_WIDTH);
		shader.SetFloat("screenHeight", SCREEN_HEIGHT);

		GLFW::GetInstance().Render();

		glfwPollEvents();
	}

	GLFW::GetInstance().Terminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}