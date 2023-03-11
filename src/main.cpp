#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <iostream>
#include <cmath>
#include <shader.h>

#define GL_WINDOW_WIDTH 640
#define GL_WINDOW_HEIGHT 640

// TODO: move elsewhere(public facing[include directory] header)
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

float vertices[] = {
     // positions         // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};  // TODO: custom model implementation or generic vec3 class."

unsigned int indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3  // second triangle
};

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

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind "VBO" to use it on GL_ARRAY_BUFFER calls.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex position attribute.
    // function arguments(in reference to the vertex attribute): location(which attribute), size, data type, normalize data flag, stride(space between consecutive attributes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // vertex color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // vertex texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // bind VBO and VAO buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // antennae texture.
    // TODO: add texture class(similar implementation to shader class)
    unsigned int texture0;
    glGenTextures(1, &texture0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);  // bind texture0 to unit 0.
    
    // texture0 options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // set x axis to use mirrored repeating textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);  // set y axis to use mirrored repeating textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // set minifying operations to use linear interpolation of the two closest mipmaps.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // set magnifying operations to use bilinear filtering(no mipmap operations as they are not used in scaling up).

    int texture0_width, texture0_height, num_color_channels0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image_data0 = stbi_load("/home/lucas/opengl-learning/assets/antennae.jpg", &texture0_width, &texture0_height, &num_color_channels0, 0);

    if (image_data0) {
        // generate texture using image data.
        // function arguments(in reference to the texture): target(generate texture on currently bound texture object), mipmap level, format(RGB[A]), width, height, 0(legacy stuff), format, datatype, image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture0_width, texture0_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data0);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE0::IMAGE::LOAD_FAILED\n" << std::endl;
    }
    stbi_image_free(image_data0);

    // wizard texture.
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);  // bind texture1 to unit 0.
    
    // texture1 options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // set x axis to use mirrored repeating textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);  // set y axis to use mirrored repeating textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // set minifying operations to use linear interpolation of the two closest mipmaps.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // set magnifying operations to use bilinear filtering(no mipmap operations as they are not used in scaling up).

    int texture1_width, texture1_height, num_color_channels1;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image_data1 = stbi_load("/home/lucas/opengl-learning/assets/wizard.png", &texture1_width, &texture1_height, &num_color_channels1, 0);

    if (image_data1) {
        // generate texture using image data.
        // function arguments(in reference to the texture): target(generate texture on currently bound texture object), mipmap level, format(RGB[A]), width, height, 0(legacy stuff), format, datatype, image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture1_width, texture1_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE1::IMAGE::LOAD_FAILED\n" << std::endl;
    }
    stbi_image_free(image_data1);

    our_shader.use();
    our_shader.set_int("texture0", 0);
    our_shader.set_int("texture1", 1);
    
    // main "render loop".
    while (!glfwWindowShouldClose(window)) {
        // TODO: process_input function for generic input handling?
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X))  // if "x" key pressed.
            glfwSetWindowShouldClose(window, true);

        // fill the viewport with a RGB color.
        glClearColor(140.0f/255.0f, 140.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate and bind the textures.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        
        // render the triangle.
        our_shader.use();
        glBindVertexArray(VAO);
        // function arguments: primitive type, number of vertices/elements, indice type, indice storage offset
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);  // swap the (color) front(final output) and back(drawn/rendered to) buffers to prevent display flickering problems.
        glfwPollEvents();  // poll for input(keyboard/mouse) events, update the window state, call callbacks, etc.
    }

    // de-allocate uneeded resources.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // terminate GLFW gracefully.
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
 {
    glViewport(0, 0, width, height);
}
