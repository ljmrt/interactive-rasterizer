#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <shader.h>

#define GL_WINDOW_WIDTH 640
#define GL_WINDOW_HEIGHT 640

// TODO: move elsewhere(public facing[include directory] header)
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

float triangle_vertices[] = {
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom right
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  // bottom left
    0.0f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f  // top
};  // TODO: custom triangle implementation or generic vec3 class."

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, "interactive-rasterizer", NULL, NULL);  // function arguments(in reference to the window): width, height, name, ?, ?.
    if (window == nullptr) {
        std::cerr << "ERROR::GLFW::WINDOW::INITIALIZATION_FAILED\n" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    int gladInitResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);  // load the address of the OS-specific OpenGL function pointers.
    if (!gladInitResult) {
        std::cerr << "ERROR::GLAD::POINTERS::INITIALIZATION_FAILED\n" << std::endl;
        return -1;
    }

    glViewport(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // set GLFW to call frame buffer "resize" function for the viewport whenever the window/framebuffer's size is modified.

    unsigned int VBO;  // vertex buffer object.
    glGenBuffers(1, &VBO);

    shader our_shader("/home/lucas/opengl-learning/include/shaders/vertex.vs", "/home/lucas/opengl-learning/include/shaders/fragment.fs");

    unsigned int VAO;  // vertex array object.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind "VBO" to use it on GL_ARRAY_BUFFER calls.
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    // vertex position attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // function arguments(in reference to the vertex attribute): location(which attribute), size, data type, normalize data flag, stride(space between consecutive attributes),
    glEnableVertexAttribArray(0);
    
    // vertex color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind VBO and VAO buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // main "render loop".
    while (!glfwWindowShouldClose(window)) {
        // TODO: process_input function for generic input handling?
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X))  // if "x" key pressed.
            glfwSetWindowShouldClose(window, true);

        // fill the viewport with a RGB color.
        glClearColor(140.0f/255.0f, 140.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle.
        our_shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // function arguments(in reference to the vertex array): primitive type, starting index, amount of vertices to draw
        
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
