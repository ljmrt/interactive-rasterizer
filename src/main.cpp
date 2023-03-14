 #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <shader.h>
#include <texture.h>
#include <string>
#include <defaults.h>

// TODO: move to defaults namespace
#define GL_WINDOW_WIDTH 640
#define GL_WINDOW_HEIGHT 640

void process_input(GLFWwindow *window, float camera_speed, float delta_time, glm::vec3& camera_position, glm::vec3 camera_front, glm::vec3 camera_up);
// TODO: move elsewhere(public facing[include directory] header)
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);

float plane_vertices[] = {
     // positions         // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};  // TODO: custom model implementation or generic vec3 class."

float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cube_positions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

// nooo! global variables!
float yaw = -90.0f;
float pitch = 0;
bool first_mouse_callback = true;
float last_x = GL_WINDOW_WIDTH / 2.0;
float last_y = GL_WINDOW_HEIGHT / 2.0;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, defaults::project_name, NULL, NULL);  // function arguments(in reference to the window): width, height, name, ?, ?.
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

    // OpenGL configuration.
    glViewport(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);  // enable z-buffer/depth testing functionality.

    // GLFW configuration.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // hide and capture the cursor.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // set GLFW to call frame buffer "resize" function for the viewport whenever the window/framebuffer's size is modified.
    glfwSetCursorPosCallback(window, mouse_callback);  // update yaw and pitch dependent on mouse move.

    unsigned int VBO;  // vertex buffer object.
    glGenBuffers(1, &VBO);

    shader our_shader(((std::string)defaults::project_directory + "include/shaders/vertex.vs").c_str(), ((std::string)defaults::project_directory + "include/shaders/fragment.fs").c_str());

    unsigned int VAO;  // vertex array object.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind "VBO" to use it on GL_ARRAY_BUFFER calls.
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    // vertex position attribute.
    // function arguments(in reference to the vertex attribute): location(which attribute), size, data type, normalize data flag, stride(space between consecutive attributes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture antennae_texture(((std::string)defaults::project_directory + "assets/antennae.jpg").c_str());
    texture wizard_texture(((std::string)defaults::project_directory + "assets/wizard.png").c_str());
    
    our_shader.use();
    our_shader.set_int("texture0", 0);
    our_shader.set_int("texture1", 1);

    // set up the "camera".
    /* glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::normalize(camera_position - camera_target);  // vector of the "opposite" of our camera's actual direction.
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
    glm::vec3 camera_up = glm::cross(camera_direction, camera_right); */
    
    // set up camera coordinate system.
    glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, camera_up));

    // set the projection coordinate/space matrix.
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)GL_WINDOW_WIDTH / (float)GL_WINDOW_HEIGHT, 0.1f, 100.0f);

    // set the corresponding uniform.
    unsigned int projection_location = glGetUniformLocation(our_shader.ID, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

    float delta_time = 0.0f;  // time between the current frame and the last frame.
    float last_frame = 0.0f;  // time of the last frame.
    
    // main "render loop".
    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        process_input(window, 8.0f, delta_time, camera_position, glm::vec3(camera_front.x, 0.0f, camera_front.z), camera_up);  // test for window close key and camera movement keys.

        // fill the viewport with a RGB color.
        glClearColor(140.0f/255.0f, 140.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f);
        glClear(GL_COLOR_BUFFER_BIT);  // clear the color buffer.

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear the depth(z) buffer.

        // bind the textures.
        antennae_texture.bind();
        wizard_texture.bind();
        
        // "activate" the shader program.
        our_shader.use();

        // set up the mouse-based direction.
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_front = glm::normalize(direction);  // update camera_front.
        
        // set the view matrix according to camera variables.
        glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
        unsigned int view_location = glGetUniformLocation(our_shader.ID, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        
        glBindVertexArray(VAO);
        // render 10 cubes.
        for (unsigned int i = 0; i < 10; i++) {
            // set model matrix.
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);  // translate the cube to the specified position.
            float angle = 20.0f * i;
            model = glm::rotate(model, ((float)glfwGetTime() * glm::radians(angle)), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(our_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));  // set model uniform.

            // render cube.
            // function arguments: primitive type, starting index, vertice count.
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glfwSwapBuffers(window);  // swap the (color) front(final output) and back(drawn/rendered to) buffers to prevent display flickering problems.
        glfwPollEvents();  // poll for input(keyboard/mouse) events, update the window state, call callbacks, etc.
    }

    // de-allocate uneeded resources.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // terminate GLFW gracefully.
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window, float camera_speed, float delta_time, glm::vec3& camera_position, glm::vec3 camera_front, glm::vec3 camera_up)
{
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, camera_up));
    const float delta_speed = camera_speed * delta_time;  // use delta time to avoid varying speeds depending on frame rate.
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X))  // close window.
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))  // move camera forward.
    {
        camera_position += camera_front * delta_speed;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))  // move camera backward.
    {
        camera_position -= camera_front * delta_speed;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))  // move camera left.
    {
        camera_position -= camera_right * delta_speed;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))  // move camera right.
    {
        camera_position += camera_right * delta_speed;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))  // move camera up.
    {
        camera_position += camera_up * delta_speed;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  // move camera down.
    {
        camera_position -= camera_up * delta_speed;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    if (first_mouse_callback) {  // prevent view snapping.
        last_x = x_pos;
        last_y = y_pos;
        first_mouse_callback = false;
    }
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;  // reversed since y-coordinates range from bottom to top.
    last_x = x_pos;
    last_y = y_pos;

    const float sensitivity = 0.06f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;
    
    yaw += x_offset;
    pitch += y_offset;
    // pitch constraints to prevent strange behavior.
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }
}
