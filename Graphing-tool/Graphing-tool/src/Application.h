#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "ComputeShader.h"
#include "Camera.h"
#include "Callbacks.h"
#include "Interpreter.h"

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "imgui/imgui_stdlib.h"


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


class Application
{
public:
	Application(const int width, const int height, std::string function);
	~Application();

	// Starts the program
	int Start();

private:
	Camera camera;
	const int WIDTH;
	const int HEIGHT;
	std::string function;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	float graphWidth = 1.0f;
	float generatedGraphWidth = 0.0f; // Holds the old value of graphWidth if it changes
	float scale = 3.0f;
	float generatedScale = 0.0f; // Holds the old value of scale if it changes
	unsigned int size = 400;

	// Buffers for the mesh data
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	// Mesh data buffers
	unsigned int heightsSSBO = 0;
	unsigned int verticesSSBO = 0;
	unsigned int indicesSSBO = 0;

	// Initialise and configure GLFW
	void initialiseGLFW();

	// Create a window using GLFW
	GLFWwindow* createGLFWWindow(int width, int height, const char* name);

	// Initialise GLAD
	bool initialiseGLAD();

	// Draw the axes
	void drawAxes(unsigned int VAO, Shader* shader, Camera* camera);


	// Calculate the actual graph data using the input function
	// Returns whether the data was updated
	bool calculate(ComputeShader* computeShader, unsigned int ssbo, bool forceRun);





	// Generate the grid mesh with the given compute shader
	void generateGridMesh(ComputeShader* generatorComputeShader, ComputeShader* calculatorComputeShader);

	// Generate a VAO for the axes
	unsigned int generateAxesVAO();

	// Modify the input array such that it is a grid
	void generateGrid(std::vector<float>* vertices, int size);
	void generateGridIndices(std::vector<unsigned int>* indices, int x, int y);
	void generateGridGPU(ComputeShader* computeShader, std::vector<float>* vertices, std::vector<unsigned int>* indices, int size);
};

#endif