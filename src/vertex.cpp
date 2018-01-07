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
#include "vertex.h"

Vertex::Vertex(float pos_x, float pos_y, float pos_z) {
    this->has_color = false;
    this->has_texture_coords = false;
    this->has_normal = false;

    this->position = glm::vec3(pos_x, pos_y, pos_z);
}

Vertex::Vertex(float pos_x, float pos_y, float pos_z, float tex_x, float tex_y) {
    this->has_color = false;
    this->has_texture_coords = true;
    this->has_normal = false;

    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->texture_coords = glm::vec2(tex_x, tex_y);
}

Vertex::Vertex(float pos_x, float pos_y, float pos_z, float r, float g, float b) {
    this->has_color = true;
    this->has_texture_coords = false;
    this->has_normal = false;

    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->color = glm::vec3(r, g, b);
}

Vertex::Vertex(float pos_x, float pos_y, float pos_z, float r, float g, float b, float tex_x, float tex_y) {
    this->has_color = true;
    this->has_texture_coords = true;
    this->has_normal = false;

    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->color = glm::vec3(r, g, b);
    this->texture_coords = glm::vec2(tex_x, tex_y);
}
