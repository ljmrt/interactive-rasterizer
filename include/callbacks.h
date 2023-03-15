#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace callbacks
{
    // called during framebuffer(window) resize.
    // note: used only in glfwSetFramebufferSizeCallback.
    //
    // @param window GLFW window to monitor the framebuffer from.
    // @param 
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    // called during mouse movement.
    // note: used only in glfwSetCursorPosCallback.
    //
    // @param window GLFW window to monitor mouse movement from.
    // @param x_pos the current x position of the mouse on screen.
    // @param y_pos the current y position of the mouse on screen.
    void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);
}

#endif  // CALLBACKS_H
