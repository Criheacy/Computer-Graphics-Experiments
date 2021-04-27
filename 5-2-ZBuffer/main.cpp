#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics.h"
#include "graphics/cube.h"
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
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	Shader shader("../shader/shader.vs",
	              "../shader/shader.gs",
	              "../shader/phong.fs");

	Cube cube = Cube(1.0f);
	// Tetrahedron tet = Tetrahedron(0.7f);

	//	cube.LogTest();
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
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// shader.SetFloat("aOffset", mOffset);
//		shader.SetMat4("projection", proj);

		float timeValue = (float)glfwGetTime();

		glm::mat4 view = glm::mat4(1.0);
		view = glm::rotate(view, glm::radians(timeValue * 20), glm::vec3(1.0f, 1.0f, 1.0f));

		shader.Activate();
		shader.SetMat4("view", view);

		shader.SetVec4("mainColor", glm::vec4(0.0f, 0.3f, 0.8f, 1.0f));
		shader.SetVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader.SetVec3("lightDirection", glm::normalize(glm::vec3(0.2f, 0.1f, 1.0f)));
		shader.SetVec3("viewPosition", glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
		shader.SetFloat("screenWidth", SCREEN_WIDTH);
		shader.SetFloat("screenHeight", SCREEN_HEIGHT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indicesNumber * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}