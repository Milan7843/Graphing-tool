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

	float scale = 3.0f;

	// Initialises and configures GLFW
	void initialiseGLFW();

	// Creates a window using GLFW
	GLFWwindow* createGLFWWindow(int width, int height, const char* name);

	// Initialises GLAD
	bool initialiseGLAD();

	// Draws the axes
	void drawAxes(unsigned int VAO, Shader* shader, Camera* camera);

	// Generates a VAO for the axes
	unsigned int generateAxesVAO();

	// Modifies the input array such that it is a grid
	void generateGrid(std::vector<float>* vertices, int size);
	void generateGridIndices(std::vector<unsigned int>* indices, int x, int y);
	void generateGridGPU(ComputeShader* computeShader, std::vector<float>* vertices, std::vector<unsigned int>* indices, int size);
};

#endif