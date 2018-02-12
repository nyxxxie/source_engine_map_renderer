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

#ifndef MESH_H
#define MESH_H

#include <string>
#include <exception>
#include <initializer_list>
#include <glad/glad.h>
#include "vertex.h"

class MeshException : public std::exception {
public:
    MeshException(std::string msg) {
        this->msg = msg;
    }

    const char* what() const throw() {
        return this->msg.c_str();
    }

private:
    std::string msg;
};


class Mesh {
public:
    Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<GLuint> indices = {});

    void Render();

private:
    bool has_indices;
    GLuint draw_count;
    GLuint vertex_array_object;
    GLuint vertex_data_buffer;
    GLuint vertex_index_buffer;
    GLenum draw_mode;
};

#endif // MESH_H
