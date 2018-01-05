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
#include <glad/glad.h>
#include "vertex.h"


Vertex::Vertex(const glm::vec3& position) {
    this->position = position;
    this->has_color = false;
    this->has_texture_coords = false;
    this->has_normal = false;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& color) {
    this->position = position;
    this->color = color;
    this->has_color = true;
    this->has_texture_coords = false;
    this->has_normal = false;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& texture_coords) {
    this->position = position;
    this->texture_coords = texture_coords;
    printf("%f\n", this->texture_coords.x);
    this->has_color = false;
    this->has_texture_coords = true;
    this->has_normal = false;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& color, const glm::vec2& texture_coords) {
    this->position = position;
    this->color = color;
    this->texture_coords = texture_coords;
    this->has_color = false;
    this->has_texture_coords = true;
    this->has_normal = false;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& color, const glm::vec2& texture_coords, const glm::vec3& normal) {
    this->position = position;
    this->color = color;
    this->texture_coords = texture_coords;
    this->normal = normal;
    this->has_color = true;
    this->has_texture_coords = true;
    this->has_normal = true;
}
