// This shader contains the code necessary to render a basic colored, textured,
// and lighted object in 3d space.  It is meant to be used for standard objects
// rendered by the engine and to be used as a base for more complex shaders
// implementing interesting effects.  The Shader class in the engine code is
// designed to work with a shader designed in this format, making it a good
// idea to use this shader code for that purpose.


#ifdef VERTEX_SHADER
// Attributes that the engine Shader class will pass to this shader.  Do NOT
// change the location numbers; they are expected by the engine Shader class.
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texture_coords;
layout (location = 2) in vec3 vertex_normal;

// Primary matrix transforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Stuff to pass to fragment shader
out vec2 texture_coords;
out vec3 normal;

void main() {
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    //vcolor = vertex_color;
    texture_coords = vertex_texture_coords;
    normal = vertex_normal;
}
#endif


#ifdef FRAGMENT_SHADER
// Stuff recieved from vertex shader
in vec2 texture_coords;
in vec3 normal;

// Texture to render with
uniform sampler3D texture_data;

// Lighting coloring
uniform vec3 object_color;
uniform vec3 light_color;

// Color that will be assigned to the fragment this shader is processing
out vec4 final_color;

const float ambient_strength = 0.1;

void main() {
    vec3 ambient = ambient_strenght * light_color;
    final_color = vec4(ambient * object_color, 1.0);  // Assign color and apply ambient light strength
    final_color *= texture(texture_data, texture_coords);  // Apply texture color
}
#endif
