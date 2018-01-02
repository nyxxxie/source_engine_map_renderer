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
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "graphics/stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char* vertex_shader1_source =
    "#version 330 core\n"
    "in vec3 pos;\n"
    "out vec4 vertex_color;"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
    "    vertex_color = vec4(0.0f, 1.0f, 0.5f, 1.0f);\n"
    "}\n";

const char* vertex_shader2_source =
    "#version 330 core\n"
    "in vec3 pos;\n"
    "in vec3 col;\n"
    "uniform float x_offset;\n"
    "uniform float y_offset;\n"
    "out vec4 vertex_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(pos.x + x_offset, pos.y + y_offset, pos.z, 1.0f);\n"
    "    //vertex_color = vec4(col, 1.0f);\n"
    "    vertex_color = vec4(col, 1.0f) + gl_Position;\n"
    "}\n";

const char* fragment_shader1_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n" ;

const char* fragment_shader2_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertex_color;"
    "void main() {\n"
    "    FragColor = vertex_color;\n"
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
 * Given a list of verticies and indices, creates a vector array object.
 * This vector array object can be bound and rendered with glDrawElements.
 */
GLuint create_color_vao_from_vertices(GLuint shader_program, float vertices[], size_t vertices_size) {
    GLuint vbo, evbo, vao;
    GLint attrib_pos, attrib_col;
    /* Create objects */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &evbo);
    glBindVertexArray(vao);  // All future binds will be associated with this vertex array

    /* Copy verticies and to buffer object */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    /* Find vertex attribute */
    attrib_pos = glGetAttribLocation(shader_program, "pos");
    if (attrib_pos == -1) {
        printf("Failed to find pos attribute.\n");
        return 0;
    }

    attrib_col = glGetAttribLocation(shader_program, "col");
    if (attrib_col == -1) {
        printf("Failed to find col attribute.\n");
        return 0;
    }

    /* Set vertex attributes */
    glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attrib_pos);
    glVertexAttribPointer(attrib_col, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(attrib_col);

    /* Unbind objects */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vao;
}

/**
 * Given a list of verticies and indices, creates a vector array object.
 * This vector array object can be bound and rendered with glDrawElements.
 */
GLuint create_vao_from_vertices(GLuint shader_program,
                                float vertices[], size_t vertices_size,
                                unsigned int indices[], size_t indices_size) {
    GLuint vbo, evbo, vao;
    GLint attrib_pos;
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

    /* Find vertex attribute */
    attrib_pos = glGetAttribLocation(shader_program, "pos");
    if (attrib_pos == -1) {
        printf("Failed to find pos attribute.\n");
        return 0;
    }

    /* Set vertex attributes */
    glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // associate triangle array buffer object data with attribute 0
    glEnableVertexAttribArray(attrib_pos);  // inform the shader it can now use vertex attribute 0

    /* Unbind objects */
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // attribute 0 is now stored in the vector array object, we can safely unbind
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

    GLuint shader1 = create_shader_program(vertex_shader1_source, fragment_shader1_source);
    if (shader1 == 0) {
        printf("Failed to create shader program 1, exiting...\n");
        return 1;
    }

    GLuint shader2 = create_shader_program(vertex_shader2_source, fragment_shader2_source);
    if (shader2 == 0) {
        printf("Failed to create shader program 2, exiting...\n");
        return 1;
    }

    float vertices1[] = {
        0.0f, 0.5f, 0.0f,  // t1 top right
        0.5f, 0.0f, 0.0f,  // t1 bottom right
       -0.5f, 0.0f, 0.0f,  // t1 bottom left
    };
    GLuint triangle1 = create_texture_vao_from_vertices(shader1, vertices1, sizeof(vertices1), "container.jpg");
    if (triangle1 == 0) {
        printf("Failed to create vao1, exiting...\n");
        return 1;
    }

    float vertices2[] = {
        0.0f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // t1 top right
        0.5f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f,  // t1 bottom right
       -0.5f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // t1 bottom left
    };
    GLuint triangle2 = create_color_vao_from_vertices(shader2, vertices2, sizeof(vertices2));
    if (triangle1 == 0) {
        printf("Failed to create vao2, exiting...\n");
        return 1;
    }

    /* Draw in wireframe mode */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Start render loop! */
    printf("Rendering started.\n");
    while (!glfwWindowShouldClose(window)) {
        /* Process input */
        process_input(window);

        /* Set the color the screen will clear to and then clear it */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ///* Render triangle1 */
        glUseProgram(shader1);
        glBindVertexArray(triangle1);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        /* Render triangle2 */
        glUseProgram(shader2);
        glUniform1f(glGetUniformLocation(shader2, "y_offset"), (sin(glfwGetTime() * 2) / 2.0f) + 0.3f);
        glUniform1f(glGetUniformLocation(shader2, "x_offset"), (sin(glfwGetTime() * 5) / 2.0f));
        glBindVertexArray(triangle2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Check and call events and swap the buffers */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    printf("Render loop exited, closing program.\n");

    glfwTerminate();
    return 0;
}
