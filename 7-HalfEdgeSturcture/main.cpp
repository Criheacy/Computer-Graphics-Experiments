#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics.h"
#include "graphics/cube.h"
#include "graphics/offgraphic.h"
#include "graphics/sphere.h"
#include "graphics/tetrahedron.h"
#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

int main()
{
	// initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 8);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
	                                      "Shader Test",
	                                      nullptr,
	                                      nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader shader("../shader/shader.vs",
	              "../shader/shader.gs",
	              "../shader/phong.fs");

	// Cube cube = Cube(0.8f);
	// Tetrahedron tet = Tetrahedron(0.7f);
	// Sphere sphere = Sphere(0.7f);
	OFFGraphics* offGraphics = new OFFGraphics("../res/bunny.off");

	Space::GetInstance().LogTest();


	unsigned int vertexNumber = Space::GetInstance().GetSerializedVerticesArraySize();
	float* vertex = Space::GetInstance().GetSerializedVerticesArrayPtr();

	unsigned int indicesNumber = Space::GetInstance().GetSerializedIndicesArraySize();
	int* indices = Space::GetInstance().GetSerializedIndicesArrayPtr();

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first,
	// then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(float), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNumber * sizeof(int), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue = (float)glfwGetTime();
		glm::mat4 view = glm::mat4(1.0);
		view = glm::scale(view, glm::vec3(5.0f, 5.0f, 5.0f));

		/// scale for bumpy
		// view = glm::scale(view, glm::vec3(0.1f, 0.1f, 0.1f));

		/// scale for bunny
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

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indicesNumber * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete data array and buffers, clear all previous allocated resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}