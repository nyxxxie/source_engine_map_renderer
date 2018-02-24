/*
 * source-engine-map-renderer - A toy project for rendering source engine maps
 * Copyright (C) 2018 nyxxxie
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
 * @brief Contains the vertices and other crap which makes up a map.
 *
 */

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>
#include "shader.h"
#include "mesh.h"

class BSPParser;


/**
 *
 */
class MapFace {
  public:
    MapFace(Shader* shader, std::vector<uint16_t>& indices);

    void render();

  private:
    /* Opengl objects */
    GLuint vao;
    GLuint element_bo;
    size_t index_amt;
    Shader* shader;
    glm::vec3 face_color;
};


/**
 *
 */
class Map {
  public:
    Map();

    void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    void FromBSP(BSPParser* parser);

  private:
    Shader* shader;
    GLuint vertex_bo;

    std::vector<MapFace> faces;
};

#endif // MAP_H
