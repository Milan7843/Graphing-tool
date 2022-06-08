#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

class Callbacks
{
public:
    void setCamera(Camera* camera);

    static Callbacks& getInstance();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void framebuffer_size_callbackImpl(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseCallbackImpl(GLFWwindow* window, double xpos, double ypos);

private:
    static Camera* camera;

    Callbacks(void)
    {
    }

    Callbacks(Callbacks const&);
    void operator=(Callbacks const&);
};