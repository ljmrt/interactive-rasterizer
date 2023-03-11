#version 330 core

out vec4 frag_color;
in vec3 our_color;
in vec2 texture_coordinates;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    frag_color = mix(texture(texture0, texture_coordinates), texture(texture1, texture_coordinates), 0.3) * vec4(our_color, 1.0);
}