#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class glfw_window
{
public:
    GLFWwindow *window;
    unsigned int window_width;
    unsigned int window_height;
    
    float mouse_yaw;
    float mouse_pitch;
    bool mouse_first_callback;
    float mouse_last_x;
    float mouse_last_y;
    float mouse_look_sensitivity;
    
    glfw_window(int major, int minor, int width, int height, const char *name);
};

#endif  // WINDOW_H
