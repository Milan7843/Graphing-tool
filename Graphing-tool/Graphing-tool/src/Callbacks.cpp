#include "Callbacks.h"

Camera* Callbacks::camera;

void Callbacks::setCamera(Camera* camera)
{
    Callbacks::camera = camera;
}

Callbacks& Callbacks::getInstance()
{
    static Callbacks instance;
    return instance;
}

void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    getInstance().framebuffer_size_callbackImpl(window, width, height);
}

void Callbacks::framebuffer_size_callbackImpl(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Callbacks::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    getInstance().mouseCallbackImpl(window, xpos, ypos);
}

void Callbacks::mouseCallbackImpl(GLFWwindow* window, double xpos, double ypos)
{
    camera->mouseCallback(window, xpos, ypos);
}