#ifdef VERTEX_SHADER
// Attributes that the engine Shader class will pass to this shader.  Do NOT
// change the location numbers; they are expected by the engine Shader class.
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_tex_coords;
layout (location = 2) in vec3 vertex_normal;

// Primary matrix transforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Stuff to pass to fragment shader
out vec3 frag_pos;
out vec2 tex_coords;
out vec3 normal;

void main() {
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    frag_pos = vec3(model * vec4(vertex_position, 1.0));
    tex_coords = vertex_tex_coords;
    normal = mat3(transpose(inverse(model))) * vertex_normal;
    // TODO: calculating inverse of a matrix is not efficient, calculate it once on the CPU and pass it to the shader
}
#endif


#ifdef FRAGMENT_SHADER
// Stuff recieved from vertex shader
in vec3 frag_pos;
in vec2 tex_coords;
in vec3 normal;

// Material stuff
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

// Light stuff
struct Light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

// Misc inputs
uniform vec3 view_pos;

// Color that will be assigned to the fragment this shader is processing
out vec4 final_color;

void main() {
    /* Calculate some intermediate components to lighting math */
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.pos - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    /* Calc ambient lighting component */
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));

    /* Calc diffuse lighting component */
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, tex_coords)));

    /* Calc specular lighting component */
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, tex_coords)));

    /* Determine final color */
    final_color = vec4(ambient + diffuse + specular, 1.0);
}
#endif
