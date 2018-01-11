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
    Init();
    SetPos(pos_x, pos_y, pos_z);
}

Vertex::Vertex(float pos_x, float pos_y, float pos_z, float tex_x, float tex_y) {
    Init();
    SetPos(pos_x, pos_y, pos_z);
    SetTexCoords(tex_x, tex_y);
}

Vertex::Vertex(float pos_x, float pos_y, float pos_z, float norm_x, float norm_y, float norm_z) {
    Init();
    SetPos(pos_x, pos_y, pos_z);
    SetNormals(norm_x, norm_y, norm_z);
}

Vertex::Vertex(float pos_x, float pos_y, float pos_z, float tex_x, float tex_y, float norm_x, float norm_y, float norm_z) {
    Init();
    SetPos(pos_x, pos_y, pos_z);
    SetTexCoords(tex_x, tex_y);
    SetNormals(norm_x, norm_y, norm_z);
}

void Vertex::SetPos(const float x, const float y, const float z) {
    this->position = glm::vec3(x, y, z);
}

void Vertex::SetTexCoords(const float x, const float y) {
    this->has_texture_coords = true;
    this->texture_coords = glm::vec2(x, y);
}

void Vertex::SetNormals(const float x, const float y, const float z) {
    this->has_normal = true;
    this->normal = glm::vec3(x, y, z);
}

void Vertex::Init() {
    this->has_texture_coords = false;
    this->has_normal = false;
}
