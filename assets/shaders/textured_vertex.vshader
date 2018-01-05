#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texture_coords;

out vec3 color;
out vec2 texture_coords;

void main() {
    gl_Position = vec4(vertex_position, 1.0);
    color = vertex_color;
    texture_coords = vertex_texture_coords;
}
