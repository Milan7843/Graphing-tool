#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"

const int WIDTH = 800, HEIGHT = 600;

// Initialises and configures GLFW
void initialiseGLFW();
// Creates a window using GLFW
GLFWwindow* createGLFWWindow(int width, int height, const char* name);
// Initialises GLAD
bool initialiseGLAD();
// Callback for when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Processes the input
void processInput(GLFWwindow* window);

// Main function, gets called automatically
int main()
{
	// Initialising and configuring GLFW
	initialiseGLFW();

	// Creating a window
	GLFWwindow* window = createGLFWWindow(WIDTH, HEIGHT, "Graph");
	if (window == NULL) return -1;
	glfwMakeContextCurrent(window);

	// Initialising GLAD
	if (!initialiseGLAD()) return -1;
	
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setting the callback for window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Shader shader("./src/shaders/vertexShader.shader", "./src/shaders/fragmentShader.shader");
	Shader shader("C:/Users/Milan/Documents/CSE/Graphing-tool/Graphing-tool/Graphing-tool/src/shaders/vertexShader.shader", "C:/Users/Milan/Documents/CSE/Graphing-tool/Graphing-tool/Graphing-tool/src/shaders/fragmentShader.shader");
	// Creating our vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Putting the vertices into the array buffer
	float vertices[] = {
	  // Position            Colour
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO;
	// Making a buffer with the ID in VBO
	glGenBuffers(1, &VBO);
	// Binding our new buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Binding our custom data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Binding the element buffer object
	unsigned int EBO;
	// Generating a buffer for the EBO
	glGenBuffers(1, &EBO);
	// Binding the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Inserting data into the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Telling OpenGL how to interpret the data
	// Position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	
	// Render loop: runs while the window does not close
	while (!glfwWindowShouldClose(window))
	{
		/* Input handling */

		processInput(window);

		/* RENDERING */

		// Drawing background
		glClearColor(1.0f, 0.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Binding the shader program
		shader.use();

		// Setting uniforms
		float time = glfwGetTime();

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);

		// View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		shader.setMat4("view", view);

		// Projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(70.0f), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);
		shader.setMat4("projection", projection);



		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		trans = glm::rotate(trans, time/90.0f, glm::vec3(0.0, 0.0, 1.0));

		// Binding the VAO
		glBindVertexArray(VAO);
		// Drawing
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT /* index type */, 0);
		glBindVertexArray(0);


		/* FINALIZING*/

		// Shows the current color buffer to the screen
		glfwSwapBuffers(window);
		// Checks for event input
		glfwPollEvents();
	}

	// Terminating GLFW
	glfwTerminate();
	return 0;
}

// Initialises and configures GLFW
void initialiseGLFW()
{
	// Initialises GLFW
	glfwInit();
	// Setting OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// OpenGL profile: core
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// Initialises GLAD
bool initialiseGLAD()
{
	// Initialising GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Failed to initialise: return false
		std::cout << "Failed to initialise GLAD." << std::endl;
		return false;
	}
	// Initialisation success: return true
	return true;
}

// Creates a window using GLFW
GLFWwindow* createGLFWWindow(int width, int height, const char* name)
{
	// Creating a window
	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);

	// Check if window was created succesfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		return NULL;
	}
	return window;
}


// Callback for when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Processes the input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}