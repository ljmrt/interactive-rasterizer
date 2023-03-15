#include <window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <callbacks.h>
#include <iostream>

glfw_window::glfw_window()
{
    window_width = 0;
    window_height = 0;

    mouse_yaw = 0;
    mouse_pitch = 0;
    mouse_first_callback = false;
    mouse_last_x = 0;
    mouse_last_y = 0;
    mouse_look_sensitivity = 0;
}

glfw_window::glfw_window(int major, int minor, int width, int height, const char *name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == nullptr) {
        std::cerr << "ERROR::GLFW::WINDOW::INITIALIZATION_FAILED\n" << std::endl;
        glfwTerminate();
        // return -1;
    }
    glfwMakeContextCurrent(window);

    // set commonly-used window variables.
    window_width = width;
    window_height = height;

    // configuration.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // hide and capture the cursor.
    glfwSetFramebufferSizeCallback(window, callbacks::framebuffer_size_callback);  // set GLFW to call frame buffer "resize" function for the viewport whenever the window/framebuffer's size is modified.
    glfwSetCursorPosCallback(window, callbacks::mouse_callback);  // update yaw and pitch dependent on mouse move.

    // set commonly-used mouse variables.
    mouse_yaw = -90.0f;
    mouse_pitch = 0.0f;
    mouse_first_callback = true;
    mouse_last_x = (float)width / 2.0;
    mouse_last_y = (float)height / 2.0;
    mouse_look_sensitivity = 0.06f;
}
