#ifdef VERTEX_SHADER
// Attributes that the engine Shader class will pass to this shader.  Do NOT
// change the location numbers; they are expected by the engine Shader class.
layout (location = 0) in vec3 vertex_position;

// Primary matrix transforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
#endif


#ifdef FRAGMENT_SHADER
// Color that will be assigned to the fragment this shader is processing
out vec4 final_color;

void main() {
    final_color = vec4(1.0);
}
#endif
