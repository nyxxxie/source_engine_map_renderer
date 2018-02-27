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
#include <GLFW/glfw3.h>
#include "bsp_parser.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "vertex.h"
#include "texture.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define CAMERA_FOV 45.0f

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float delta_time = 0.0f;
float last_frame = 0.0f;
float last_x = 0.0f;
float last_y = 0.0f;
int draw_mode = 0;


/**
 * Called to resize the opengl viewport when the window dimensions are altered
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * Processes input key presses
 */
void key_callback(GLFWwindow* window, int key, int scode, int action, int mod) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }
        if (key == GLFW_KEY_F) {
	    draw_mode = (draw_mode + 1) % 3;
	    switch(draw_mode) {
            case 0:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
            case 1:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
            case 2:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	    }
	}

        bool sprint = bool(mod | GLFW_MOD_SHIFT);
    }
}

void process_input(GLFWwindow* window) {
    bool sprint = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
               	   glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::FORWARD, delta_time, sprint);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, delta_time, sprint);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::LEFT, delta_time, sprint);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::RIGHT, delta_time, sprint);
}

/**
 * Callback for mouse events
 */
void mouse_callback(GLFWwindow* window, double pos_x, double pos_y) {
    /* If this is the first call */
    static bool first_call = true;
    if (first_call) {
        last_x = pos_x;
        last_y = pos_y;
        first_call = false;
    }

    /* Figure out offset coordinates */
    float x_offset = pos_x - last_x;
    float y_offset = last_y - pos_y;

    /* Save current position values for later */
    last_x = pos_x;
    last_y = pos_y;

    /* Process camera movement */
    camera.ProcessMouseMovement(x_offset, y_offset);
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

    /* Set input mode and user input event callbacks */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    Texture container_texture("./assets/textures/container2.png");
    Texture container_texture_specular("./assets/textures/container2_specular.png");

    /* Inform OpenGL we'd like to enable depth testing */
    glEnable(GL_DEPTH_TEST);

    Shader object_shader("./assets/shaders/base.glsl");
    Shader lamp_shader("./assets/shaders/lamp.glsl");
    Mesh box({
        /* Back */
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f),

        /* Front */
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f),

        /* Left */
        Vertex(-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f),

        /* Right */
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f),

        /* Bottom */
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f),

        /* Top */
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f)
    });
    Mesh textured_box({
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f),

        Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f),

        Vertex(-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f),

        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f),

        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f),
        Vertex( 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f),
        Vertex( 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f),
        Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f),
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f),

        Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f),
        Vertex( 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),
        Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),
        Vertex(-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f)
    });
    glm::vec3 light_pos = glm::vec3(1.2f, 0.7f, 2.0f);

    /* Parse BSP file if one is provided */
    Map* map = nullptr;
    if (argc >= 2) {
        printf("Parsing BSP file at \'%s\'\n", argv[1]);
        BSPParser parser(argv[1]);
        map = new Map();
        map->FromBSP(&parser);
    }

    /* Start render loop! */
    printf("Rendering started.\n");
    while (!glfwWindowShouldClose(window)) {
        /* Calculate delta_time so that we can smooth movement */
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        light_pos.x = cos(glfwGetTime()) * 1.5f;
        light_pos.z = sin(glfwGetTime()) * 1.5f;

        /* Process keyboard input */
        process_input(window);

        /* Set the color the screen will clear to and then clear it */
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //* Use shader and set basic shader options */
        //object_shader.Use();
        //object_shader.SetVec3("view_pos", camera.pos.x, camera.pos.y, camera.pos.z);

        ///* Set shader material */
        //object_shader.SetInt("material.diffuse", 0);
        //object_shader.SetInt("material.specular", 1);
        //object_shader.SetFloat("material.shininess", 32.0f);

        ///* Set shader light settings */
        //object_shader.SetVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        //object_shader.SetVec3("light.diffuse",  0.7f, 0.7f, 0.7f);
        //object_shader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
        //object_shader.SetFloat("light.constant", 1.0f);
        //object_shader.SetFloat("light.linear", 0.09);
        //object_shader.SetFloat("light.quadratic", 0.032f);
        //object_shader.SetVec3("light.pos", light_pos.x, light_pos.y, light_pos.z);

        ///* Create the model, view, and projection transformation matricies */
        //glm::mat4 projection = glm::perspective(glm::radians(CAMERA_FOV),
        //                                        float(WINDOW_WIDTH)/WINDOW_HEIGHT,
        //                                        0.1f, 100.0f);
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 model = glm::mat4();
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

        ///* Set light shader uniforms and draw the light object */
        //object_shader.SetMat4("projection", projection);
        //object_shader.SetMat4("view", view);
        //object_shader.SetMat4("model", model);
        //container_texture.Use(GL_TEXTURE0);
        //container_texture_specular.Use(GL_TEXTURE1);
        //textured_box.Render();

        ///* Set the model shader to position the lamp away from the cube in the center */
        //model = glm::mat4();
        //model = glm::translate(model, light_pos);
        //model = glm::scale(model, glm::vec3(0.5f));

        ///* Set lamp shader uniforms and draw the lamp object */
        //lamp_shader.Use();
        //lamp_shader.SetMat4("projection", projection);
        //lamp_shader.SetMat4("view", view);
        //lamp_shader.SetMat4("model", model);
        //box.Render();

        /* Draw map (if it exists) */
        if (map != nullptr) {
          glm::mat4 projection = glm::perspective(glm::radians(CAMERA_FOV),
                                                  float(WINDOW_WIDTH)/WINDOW_HEIGHT,
                                                  0.1f, 1000.0f);
          glm::mat4 view = camera.GetViewMatrix();
          glm::mat4 model = glm::mat4();
          model = glm::scale(model, glm::vec3(0.005f));
          model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
          model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
          map->render(model, view, projection);
        }

        /* Check and call events and swap the buffers */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    printf("Render loop exited, closing program.\n");

    glfwTerminate();
    return 0;
}
