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
#include <glad/glad.h>
#include "primitive.h"


Primitive::Primitive(std::initializer_list<GLfloat> vertices,
                     Shader* shader, bool colored){
    GLint attrib_pos, attrib_col, success;
    GLuint stride;

    this->shader = shader;

    /* Create objects */
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &vbo);
    Bind();

    /* Copy verticies and to buffer object */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.end() - vertices.begin(),
                 vertices.begin(), GL_STATIC_DRAW);

    /* Determine stride length */
    if (colored) {
        stride = 6 * sizeof(float);
    } else {
        stride = 3 * sizeof(float);
    }

    /* Find vertex position attribute and point it to the appropriate loc in the
       buffer object */
    attrib_pos = glGetAttribLocation(shader->id(), "position");
    if (attrib_pos == -1) {
        throw PrimitiveException("Failed to find \"position\" attribute.");
    }
    glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(attrib_pos);

    /* If we are expecting colors in vertices, set the attributes properly */
    if (colored) {
        attrib_col = glGetAttribLocation(shader->id(), "color");
        if (attrib_col == -1) {
            throw PrimitiveException("Failed to find \"color\" attribute.");
        }
        glVertexAttribPointer(attrib_col, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(attrib_col);
    }

    /* Unbind objects */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    Unbind();
}

void Primitive::Render(bool use_shader){
    if (use_shader)
        glUseProgram(shader->id());
    Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Primitive::Bind() {
    glBindVertexArray(this->vao);
}

void Primitive::Unbind() {
    glBindVertexArray(0);
}
