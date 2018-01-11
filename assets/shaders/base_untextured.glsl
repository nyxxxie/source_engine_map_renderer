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
out vec3 frag_pos;

void main() {
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    frag_pos = vec3(model * vec4(vertex_position, 1.0));
    normal = mat3(transpose(inverse(model))) * vertex_normal;
    // TODO: calculating inverse of a matrix is not efficient, calculate it once on the CPU and pass it to the shader
}
#endif


#ifdef FRAGMENT_SHADER
// Stuff recieved from vertex shader
in vec3 normal;
in vec3 frag_pos;

// Lighting coloring
uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

// Color that will be assigned to the fragment this shader is processing
out vec4 final_color;

const float ambient_strength = 0.1;
const float specular_strength = 0.5;

void main() {
    /* Calculate some intermediate components to lighting math */
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    /* Calc light components */
    vec3 ambient = ambient_strength * light_color;
    vec3 diffuse = light_color * max(dot(norm, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * light_color * spec;
    vec3 face_lighting = ambient + diffuse + specular;

    /* Determine final color */
    final_color = vec4(object_color * face_lighting, 1.0);
}
#endif
