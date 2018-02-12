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

#include <initializer_list>
#include <vector>
#include <glad/glad.h>
#include "mesh.h"
#include "vertex.h"

Mesh::Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<GLuint> indices) {
    std::vector<GLfloat> vertex_data;
    std::vector<GLuint> index_data;
    bool has_texture_coords;
    bool has_normal;
    GLuint stride;
    unsigned long base_offset, pos_offset, normal_offset, texture_coords_offset;

    /* Set default draw mode */
    draw_mode = GL_TRIANGLES;

    /* Process the input indices into usable buffers */
    has_indices = false;
    for (const GLuint* it=indices.begin(); it != indices.end(); it++) {
        has_indices = true;
        index_data.push_back(*it);
    }

    /* Process the input vertices into usable buffers */
    has_texture_coords = false;
    has_normal = false;
    for (const Vertex* it=vertices.begin(); it != vertices.end(); it++) {
        vertex_data.push_back(it->position.x);
        vertex_data.push_back(it->position.y);
        vertex_data.push_back(it->position.z);

        if (it->has_texture_coords) {
            has_texture_coords = true;
            vertex_data.push_back(it->texture_coords.x);
            vertex_data.push_back(it->texture_coords.y);
        }

        if (it->has_normal) {
            has_normal = true;
            vertex_data.push_back(it->normal.x);
            vertex_data.push_back(it->normal.y);
            vertex_data.push_back(it->normal.z);
        }
    }

    if (has_indices) {
        draw_count = index_data.size();
    } else {
        draw_count = vertices.end() - vertices.begin();
    }

    /* Calculate stride and attribute offsets */
    stride = (3 * sizeof(GLfloat));
    base_offset = (3 * sizeof(GLfloat));
    pos_offset = 0;
    if (has_texture_coords) {
        texture_coords_offset = base_offset;
        base_offset += 2 * sizeof(GLfloat);
        stride += 2 * sizeof(GLfloat);
    }
    if (has_normal) {
        normal_offset = base_offset;
        base_offset += (3 * sizeof(GLfloat));
        stride += (3 * sizeof(GLfloat));
    }

    /* Create the vertex object array that'll store the mesh render info */
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    /* Create a buffer object to store vertex data in */
    glGenBuffers(1, &vertex_data_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_data_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat),
                 vertex_data.data(), GL_STATIC_DRAW);

    /* Create a buffer object to store vertex data in */
    if (has_indices) {
        glGenBuffers(1, &vertex_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     index_data.size() * sizeof(GLuint),
                     index_data.data(), GL_STATIC_DRAW);
    }

    /* Assign the position information to the first attribute */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)pos_offset);

    /* Enable and define texture coordinates vertex attribute */
    if (has_texture_coords) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)texture_coords_offset);
    }

    /* Enable and define texture coordinates vertex attribute */
    if (has_normal) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)normal_offset);
    }

    /* Unbind the vertex array object and our data buffer since we're done
       using them.  This is important, because we want to ensure that we don't
       accidentally modify either buffers in subsequent gl* api calls */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    if (has_indices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::Render() {
    glBindVertexArray(vertex_array_object);
    if (has_indices) {
        glDrawElements(draw_mode, draw_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(draw_mode, 0, draw_count);
    }
}
