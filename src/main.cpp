#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define GL_WINDOW_WIDTH 640
#define GL_WINDOW_HEIGHT 640

// TODO: move elsewhere(public facing[include directory] header)
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "layout (location = 1) in vec3 a_color;\n"
    "out vec3 our_color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(a_pos, 1.0);\n"
    "    our_color = a_color;\n"
    "}\0";
const char *fragment_shader_source = "#version 330 core\n"
    "out vec4 frag_color;\n"
    "uniform vec4 our_color;\n"
    "void main()\n"
    "{\n"
    "    frag_color = our_color;\n"
    "}\0";

float triangle_vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
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

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    
    glCompileShader(vertex_shader);
    // ensure vertex shader compilation.
    int vertex_shader_compilation;  // indictation of compilation success.
    char vertex_shader_info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compilation);  // assign success to vertex_shader_compilation.
    if (!vertex_shader_compilation) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, vertex_shader_info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertex_shader_info_log << std::endl;
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    
    glCompileShader(fragment_shader);
    // ensure fragment shader compilation.
    int fragment_shader_compilation;  // indication of compilation success.
    char fragment_shader_info_log[512];  // TODO: create global or generic info log?
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_compilation);  // assign success to fragment_shader_compilation.
    if (!fragment_shader_compilation) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, fragment_shader_info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragment_shader_info_log << std::endl;
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    
    glLinkProgram(shader_program);
    // ensure shader program linking success.
    int shader_program_linking = 0;  // indication of linking success.
    char shader_program_info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_program_linking);
    if (!shader_program_linking) {
        glGetProgramInfoLog(shader_program, 512, NULL, shader_program_info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << shader_program_info_log << std::endl;
    }

    // remove unecessary shaders.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    unsigned int VAO;  // vertex array object.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind "VBO" to use it on GL_ARRAY_BUFFER calls.
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // function arguments(in reference to the vertex attribute): location(which attribute), size, data type, normalize data flag, stride(space between consecutive attributes),
    glEnableVertexAttribArray(0);

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
        
        glUseProgram(shader_program);  // use the shader program in further shader/rendering calls.

        // calculate vertex color/update the uniform color.
        float time_value = glfwGetTime();
        float yellow_value = (sin(time_value) / 2.0f) + 0.5f;
        int vertex_color_location = glGetUniformLocation(shader_program, "our_color");
        glUniform4f(vertex_color_location, yellow_value, yellow_value, 0.0f, 1.0f);

        // render the triangle.
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
