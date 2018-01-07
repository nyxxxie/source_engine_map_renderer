#version 330 core
in vec2 texture_coords;
out vec4 final_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    final_color = mix(texture(texture1, texture_coords),
                      texture(texture2, texture_coords),
                      0.2);
}
