#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texture_coordinates;

out vec3 our_color;
out vec2 texture_coordinates;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(a_pos, 1.0);
    our_color = a_color;
    texture_coordinates = vec2(a_texture_coordinates.x, a_texture_coordinates.y);
}