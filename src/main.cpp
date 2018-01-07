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
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "mesh.h"
#include "vertex.h"
#include "texture.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up(0.0f, 1.0f, 0.0f);
float delta_time = 0.0f;
float last_frame = 0.0f;
float pitch = 0.0f;
float yaw = -90.0f;
float last_x = WINDOW_WIDTH*0.5f;
float last_y = WINDOW_HEIGHT*0.5f;

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

    float camera_speed = 7.0f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
}

/**
 * Callback for mouse events
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    /* If this is the first time running this function, set the mouse coords to
       the current frame's values */
    static bool first_run = true;
    if (first_run) {
        last_x = xpos;
        last_y = ypos;
        first_run = false;
    }

    /* Calculate movement offset between the last and current frame */
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    /* Weaken the movement speed by a sensitivity coefficient */
    float sensitivity = 0.10f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    /* Offset pitch and yaw by the calculated movement offsets */
    pitch += yoffset;
    yaw += xoffset;

    /* Lock pitch to a fixed min/max */
    if (pitch > 89.0f) { pitch = 89.0f; }
    if (pitch < -89.0f) { pitch = -89.0f; }

    /* Calculate the direction vector to point the camera in */
    camera_front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
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
    
    /* Bind mouse to the screen and register a callback when its positon
       changes */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    /* Inform OpenGL we'd like to enable depth testing */
    glEnable(GL_DEPTH_TEST);

    Shader shader1("./assets/shaders/textured_transform_vertex.vshader",
                   "./assets/shaders/textured_mixed_vertex.fshader");
    Mesh box({
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f),

        Vertex(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f, 0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),

        Vertex(-0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  1.0f, 0.0f),

        Vertex( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f),

        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  1.0f, 1.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f, 0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f, 0.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),

        Vertex(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  0.0f, 0.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f)
    });

    Texture texture1("assets/textures/container.jpg");
    Texture texture2("assets/textures/awesomeface.png", GL_TEXTURE1, GL_RGBA, true);

    /* Draw in wireframe mode */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
    /* Start render loop! */
    printf("Rendering started.\n");
    while (!glfwWindowShouldClose(window)) {
        /* Calculate delta_time so that we can smooth movement */
        float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

	/* Calculate an evolving time value to play with */
	float cycling_val = (sin(glfwGetTime() - glm::radians(90.0f)) + 1.0f) * 0.5f;
	float rotating_val = float(glfwGetTime());

        /* Process keyboard input */
        process_input(window);

        /* Set the color the screen will clear to and then clear it */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        /* Render triangle */
	texture1.Use();
	texture2.Use();
	shader1.Use();
	shader1.SetUniform("texture1", 0);
	shader1.SetUniform("texture2", 1);

	/* Create the projection matrix and send it to the vertex shader */
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), float(WINDOW_WIDTH)/WINDOW_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(shader1.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	/* Create the view matrix and send it to the vertex shader */
        glm::mat4 view;
	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        glUniformMatrix4fv(shader1.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));

	std::vector<glm::vec3> cube_positions = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };

	for (int i = 0; i < cube_positions.size(); i++) {
            glm::mat4 model;
	    model = glm::translate(model, cube_positions[i]);
            model = glm::rotate(model, (rotating_val / 5.0f) * glm::radians(50.0f * i), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, (rotating_val / 5.0f) * glm::radians(70.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
	    glUniformMatrix4fv(shader1.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	    box.Render();
	}

        /* Check and call events and swap the buffers */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    printf("Render loop exited, closing program.\n");

    glfwTerminate();
    return 0;
}
