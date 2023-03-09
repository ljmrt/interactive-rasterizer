#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define GL_WINDOW_WIDTH 640
#define GL_WINDOW_HEIGHT 640

// TODO: move elsewhere(public facing[include directory] header)
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, "interactive-rasterizer", NULL, NULL);  // function arguments(in reference to the window): width, height, name, ?, ?.
    if (window == nullptr) {
        std::cerr << "GLFW Initialization Error: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    int gladInitResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);  // load the address of the OS-specific OpenGL function pointers.
    if (!gladInitResult) {
        std::cerr << "GLAD Initialization Error: Failed to load OpenGL function pointer addresses." << std::endl;
        return -1;
    }

    glViewport(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // set GLFW to call frame buffer "resize" function for the viewport whenever the window/framebuffer's size is modified.

    // main "render loop"
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);  // swap the (color) front(final output) and back(drawn/rendered to) buffers to prevent display flickering problems.
        glfwPollEvents();  // poll for input(keyboard/mouse) events, update the window state, call callbacks, etc.
    }

    // terminate GLFW gracefully.
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
