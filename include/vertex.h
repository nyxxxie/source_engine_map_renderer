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

#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <exception>
#include <initializer_list>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"


class Vertex {
public:
    Vertex(float pos_x, float pos_y, float pos_z);
    Vertex(float pos_x, float pos_y, float pos_z, float tex_x, float tex_y);
    Vertex(float pos_x, float pos_y, float pos_z, float norm_x, float norm_y, float norm_z);
    Vertex(float pos_x, float pos_y, float pos_z, float tex_x, float tex_y, float norm_x, float norm_y, float norm_z);

    void SetPos(const float x, const float y, const float z);
    void SetTexCoords(const float x, const float y);
    void SetNormals(const float x, const float y, const float z);

    glm::vec3 position;
    glm::vec2 texture_coords;
    bool has_texture_coords;
    glm::vec3 normal;
    bool has_normal;

private:
    void Init();
};

#endif // VERTEX_H
