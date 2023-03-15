#include <defaults.h>
#include <window.h>

const char *defaults::project_name = "opengl-learning";
const char *defaults::project_directory = "/home/lucas/programming/graphics/opengl-learning/";
const unsigned int defaults::window_width = 640;
const unsigned int defaults::window_height = 640;
glfw_window defaults::active_window(3, 3, 10, 10, "default window");  // not default parameters due to easier debugging.
