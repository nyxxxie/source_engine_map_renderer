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
#include "shader.h"
#include "primitive.h"

#define STB_IMAGE_IMPLEMENTATION
#include "graphics/stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/**
 * Called to resize the opengl viewport when the window dimensions are altered
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
j * Processes input key presses
 */
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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

    Shader* shader = new Shader("./assets/shaders/colored_vertex.vshader",
                                "./assets/shaders/colored_vertex.fshader");
    Primitive* triangle = new Primitive({
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // t1 top right
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // t1 bottom right
       -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // t1 bottom left
    }, shader, true);

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

        /* Render triangle */
        triangle->Render(false);

        /* Check and call events and swap the buffers */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    printf("Render loop exited, closing program.\n");

    glfwTerminate();
    return 0;
}
