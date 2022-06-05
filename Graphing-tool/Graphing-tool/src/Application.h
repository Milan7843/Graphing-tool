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

#include "Shader.h"
#include "Camera.h"
#include "Callbacks.h"

class Application
{
public:
	Application(const int width, const int height, std::string function, const float scrollSens);
	~Application();

	// Starts the program
	int Start();

private:
	Camera camera;
	const int WIDTH;
	const int HEIGHT;
	std::string function;
	const float scrollSens;

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
	void generateGrid(float in[], int x, int y);
	void generateGridIndices(unsigned int in[], int x, int y);
};

#endif