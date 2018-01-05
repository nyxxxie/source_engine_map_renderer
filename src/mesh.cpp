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
#include <initializer_list>
#include <vector>
#include <glad/glad.h>
#include "mesh.h"
#include "vertex.h"

Mesh::Mesh(std::initializer_list<Vertex> vertices) {
    std::vector<GLfloat> vertex_positions;
    bool has_color;
    GLuint stride;

    draw_count = vertices.end() - vertices.begin();

    /* Process the input vertices into usable buffers */
    for (const Vertex* it=vertices.begin(); it != vertices.end(); it++) {
        vertex_positions.push_back(it->position.x);
        vertex_positions.push_back(it->position.y);
        vertex_positions.push_back(it->position.z);

	if (it->has_color) {
            has_color = true;
            vertex_positions.push_back(it->color.r);
            vertex_positions.push_back(it->color.g);
            vertex_positions.push_back(it->color.b);
	}
    }

    /* Figure out what stride we should use */
    if (has_color) {
        stride = 6 * sizeof(GLfloat);
    } else {
        stride = 3 * sizeof(GLfloat);
    }

    /* Create the vertex object array that'll store the mesh render info */
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    /* Save the input vertex info to the GPU */
    glGenBuffers(1, &vertex_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(GLfloat),
		 vertex_positions.data(), GL_STATIC_DRAW);

    /* Assign the position information to the first attribute */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    /* If we have a color assign the attribute information */
    if (has_color) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
    }

    /* Unbind the vertex object array and all buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Render() {
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, draw_count);
}
