#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <window.h>

namespace defaults
{
    extern const char *project_name;  // project name.
    extern const char *project_directory;  // the root directory of the project(ex: ~/programming/project/)
    extern const unsigned int window_width;  // width of the window(usually the same width as the viewport).
    extern const unsigned int window_height;  // height of the window(usually the same height as the viewport).
    extern glfw_window active_window;  // current window.
}

#endif  // DEFAULTS_H
