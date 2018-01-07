#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec2 vertex_texture_coords;

out vec2 texture_coords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    texture_coords = vertex_texture_coords;
}
