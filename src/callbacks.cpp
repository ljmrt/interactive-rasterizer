#include <callbacks.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <defaults.h>

void callbacks::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void callbacks::mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    if (defaults::active_window.mouse_first_callback) {  // prevent view snapping.
        defaults::active_window.mouse_last_x = x_pos;
        defaults::active_window.mouse_last_y = y_pos;
        defaults::active_window.mouse_first_callback = false;
    }
    float x_offset = x_pos - defaults::active_window.mouse_last_x;
    float y_offset = defaults::active_window.mouse_last_y - y_pos;  // reversed since y-coordinates range from bottom to top.
    defaults::active_window.mouse_last_x = x_pos;
    defaults::active_window.mouse_last_y = y_pos;

    x_offset *= defaults::active_window.mouse_look_sensitivity;
    y_offset *= defaults::active_window.mouse_look_sensitivity;
    
    defaults::active_window.mouse_yaw += x_offset;
    defaults::active_window.mouse_pitch += y_offset;
    // mouse_pitch constraints to prevent strange behavior.
    if (defaults::active_window.mouse_pitch > 89.0f) {
        defaults::active_window.mouse_pitch = 89.0f;
    }
    if (defaults::active_window.mouse_pitch < -89.0f) {
        defaults::active_window.mouse_pitch = -89.0f;
    }
}
