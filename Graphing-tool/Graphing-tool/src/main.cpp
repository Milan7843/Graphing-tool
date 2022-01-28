#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.);\n"
"}\0";


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

	// Putting the vertices into the array buffer
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO;
	// Making a buffer with the ID in VBO
	glGenBuffers(1, &VBO);
	// Binding our new buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Binding our custom data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Render loop: runs while the window does not close
	while (!glfwWindowShouldClose(window))
	{
		/* Input handling */

		processInput(window);

		/* RENDERING */

		glClearColor(0.8f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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