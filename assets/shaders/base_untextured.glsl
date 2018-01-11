#ifdef VERTEX_SHADER
// Attributes that the engine Shader class will pass to this shader.  Do NOT
// change the location numbers; they are expected by the engine Shader class.
layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec3 vertex_normal;

// Primary matrix transforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Stuff to pass to fragment shader
out vec3 normal;

void main() {
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    normal = vertex_normal;
}
#endif


#ifdef FRAGMENT_SHADER
// Stuff recieved from vertex shader
in vec3 normal;

// Lighting coloring
uniform vec3 object_color;
uniform vec3 light_color;

// Color that will be assigned to the fragment this shader is processing
out vec4 final_color;

const float ambient_strength = 0.1;

void main() {
    vec3 ambient = ambient_strength * light_color;
    final_color = vec4(ambient * object_color, 1.0);  // Assign color and apply ambient light strength
}
#endif
