#version 330 core
in vec3 color;
in vec2 texture_coords;
out vec4 final_color;

uniform sampler2D texture_data;

void main() {
    final_color = texture(texture_data, texture_coords);
}
