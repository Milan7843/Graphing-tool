#pragma once

#include <GLFW/glfw3.h>

// Matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


class Camera
{
	/* Public members*/
public:
	Camera();
	Camera(glm::vec3 pos);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(int width, int height);

	void processInput(GLFWwindow* window, float deltaTime);
	// Callback for when the mouse is moved
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);

	// Don't use the current mouse offset, but start from where the cursor is
	void resetMouseOffset();

	// Get the camera position, rotation
	glm::vec3 getPosition();
	float getPitch();
	float getYaw();

	float* getCameraSpeedPointer();
	float* getFovPointer();
	float* getSensitivityPointer();

	/* Private members */
private:
	// Camera postion data
	glm::vec3 position = glm::vec3(-1.0f, 1.0f, 0.0f);
	glm::vec3 up;
	glm::vec3 forward;
	float lastx = 400, lasty = 300;
	float yaw = 35.0f, pitch = -30.0f;
	bool firstMouse = true;
	float fov = 40.0f;
	float sensitivity = 1.0f;
	float cameraSpeed = 1.0f;
};