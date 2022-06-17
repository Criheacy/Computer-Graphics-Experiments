#include "utils/glfw_toolkit.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "graphics/cube.h"
#include "graphics/graphics.h"
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
	// Sphere sphere = Sphere(0.6f);
	OFFGraphics* offGraphics = new OFFGraphics("../res/bumpy.off");
	offGraphics->SubdivideFaces();

	// Space::GetInstance().LogTest();

	unsigned int vertexCount = Space::GetInstance().GetSerializedVerticesArraySize();
	float *vertexArray = Space::GetInstance().GetSerializedVerticesArrayPtr();

	unsigned int indexCount = Space::GetInstance().GetSerializedIndicesArraySize();
	int *indexArray = Space::GetInstance().GetSerializedIndicesArrayPtr();

	GLFW::GetInstance().BindVertexArray(vertexCount, vertexArray);
	GLFW::GetInstance().BindIndexArray(indexCount, indexArray);

	GLFW::GetInstance().GetInputHandler()->AddOnDragCallback([](int button, glm::vec2 startPos, glm::vec2 endPos) {
		float distance = glm::distance(startPos, endPos);
		float relative = (endPos.y - startPos.y) + (endPos.x - startPos.x);
		glm::vec3 motion = glm::vec3(endPos - startPos, 0.0);
		// adjust settings
		motion.y = -motion.y;
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			glm::mat4 view = Space::GetInstance().GetView();
			Space::GetInstance().SetView(glm::translate(view,
                glm::vec3(endPos - startPos, 0.0) / static_cast<float>(SCREEN_WIDTH)));
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			glm::mat4 view = Space::GetInstance().GetView();
			Space::GetInstance().SetView(glm::rotate(view, glm::radians(distance), motion));
		} else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			glm::mat4 view = Space::GetInstance().GetView();
			Space::GetInstance().SetView(glm::scale(view, glm::vec3(distance)));
		}
	});

	glm::mat4 viewOffset = glm::mat4(1.0);
	viewOffset = glm::scale(viewOffset, glm::vec3(0.2, 0.2, 0.2));
	//viewOffset = glm::scale(viewOffset, glm::vec3(5.0, 5.0, 5.0));

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		shader.Activate();
		shader.SetMat4("view", Space::GetInstance().GetView() * viewOffset);

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