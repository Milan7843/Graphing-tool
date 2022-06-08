#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(direction);
}
Camera::Camera(glm::vec3 pos)
{
	position = pos;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(direction);
}
Camera::~Camera()
{
	std::cout << "Camera destroyed" << std::endl;
}

glm::mat4 Camera::getViewMatrix()
{   //                           cam pos,  target,            up vector
	glm::mat4 view = glm::lookAt(position, position + forward, up);
	return view;
}
glm::mat4 Camera::getProjectionMatrix(int width, int height)
{   //                           cam pos,  target,            up vector
	glm::mat4 projection = glm::perspective(glm::radians(40.0f), (float)width / (float)height, 0.1f, 100.0f);
	return projection;
}


void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Sets the mouse offset by frame appropriately for the first frame
	if (firstMouse)
	{
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastx;
	float yoffset = lasty - ypos; // reversed since y-coordinates range from bottom to top
	lastx = xpos;
	lasty = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// Applying rotation
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Setting the new vectors
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(direction);
}

void Camera::resetMouseOffset()
{
	firstMouse = true;
}

// Processes the input
void Camera::processInput(GLFWwindow* window, float deltaTime)
{
	float realSpeed = cameraSpeed * deltaTime;
	// FORWARD/BACKWARD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += realSpeed * forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= realSpeed * forward;

	// SIDE/SIDE
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= glm::normalize(glm::cross(forward, up)) * realSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += glm::normalize(glm::cross(forward, up)) * realSpeed;

	// UP/DOWN
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		position += realSpeed * up;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		position -= realSpeed * up;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}