/*
* source-engine-map-renderer - A toy project for rendering source engine maps
 * Copyright (C) 2017 nyxxxie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @file
 * @brief Main file
 *
 * This file contains the entry point, argument parsing stuff, and other logic
 * related to starting the program.
 */

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char* vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

const char* fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n" ;


/**
 * Called to resize the opengl viewport when the window dimensions are altered
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * Processes input key presses
 */
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

/**
 * Creates a shader program from the src of a vertex shader and fragment shader.
 */
GLuint create_shader_program(const char* vertex_shader_src, const char* fragment_shader_src) {
    GLuint vertex_shader, fragment_shader, shader_program;
    int success;
    char infolog[512];

    /* Create vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, sizeof(infolog), NULL, infolog);
        glDeleteShader(vertex_shader);
        printf("Failed to compile vertex shader: \"%s\"\n", infolog);
        return 0;
    }
    printf("Vertex shader compiled.\n");

    /* Create fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(infolog), NULL, infolog);
        glDeleteShader(fragment_shader);
        printf("Failed to compile fragment shader: \"%s\"\n", infolog);
        return 0;
    }
    printf("Fragment shader compiled.\n");

    /* Create shader program */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, sizeof(infolog), NULL, infolog);
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
        printf("Failed to link shader program: \"%s\"\n", infolog);
        return 0;
    }
    printf("Shader program linked.\n");

    /* Clean up shaders and tell opengl to use the shader program we just made */
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    return shader_program;
}

/**
 * Given a list of verticies and indices, creates a vector array object.
 * This vector array object can be bound and rendered with glDrawElements.
 */
GLuint create_vao_from_vertices(float vertices[], size_t vertices_size,
                                 unsigned int indices[], size_t indices_size) {
    GLuint vbo, evbo, vao;
    /* Create objects */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &evbo);
    glBindVertexArray(vao);  // All future binds will be associated with this vertex array

    /* Copy verticies and to buffer object */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    /* Copy indices and to buffer object */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, evbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    /* Set vertex attributes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // associate triangle array buffer object data with attribute 0
    glEnableVertexAttribArray(0);  // inform the shader it can now use vertex attribute 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // attribute 0 is now stored in the vector array object, we can safely unbind

    /* Unbind objects */
    glBindVertexArray(0);  // Unbind the vertex array first so that we don't de-associate the element array when we unbind it next
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

    return vao;
}

/**
 * Entry point.
 */
int main(int argc, char* argv[]) {
    /* Init glfw */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create the gflw window */
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Source Engine Map Renderer", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    /* Have glad load our OpenGL functions */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.\n");
        return 1;
    }

    /* Inform OpenGL the dimensions of our rendering window */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    /*  Register a callback to resize the draw space when the window changes */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint shader1 = create_shader_program(vertex_shader_source, fragment_shader_source);
    if (shader1 == 0) {
        printf("Failed to create shader program, exiting...\n");
        return 1;
    }

    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    GLuint triangle1 = create_vao_from_vertices(vertices, sizeof(vertices), indices, sizeof(indices));
    if (triangle1 == 0) {
        printf("Failed to create vao, exiting...\n");
        return 1;
    }

    /* Draw in wireframe mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Start render loop! */
    printf("Rendering started.\n");
    while (!glfwWindowShouldClose(window)) {
        /* Process input */
        process_input(window);

        /* Set the color the screen will clear to and then clear it */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render triangle */
        glUseProgram(shader1);
        glBindVertexArray(triangle1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Check and call events and swap the buffers */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    printf("Render loop exited, closing program.\n");

    glfwTerminate();
    return 0;
}
