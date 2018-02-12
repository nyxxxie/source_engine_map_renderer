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

#include <stdio.h>
#include "map.h"


Map::Map() {
  shader = nullptr;
  vertex_array_object = -1;
  vertex_data_buffer = -1;
}

void Map::render(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
  shader->Use();
  shader->SetMat4("projection", projection);
  shader->SetMat4("view", view);
  shader->SetMat4("model", model);
  glBindVertexArray(vertex_array_object);
  glDrawArrays(GL_POINTS, 0, points.size());
}

void Map::bake() {
  shader = new Shader("./assets/shaders/point_cloud.glsl");

  /* Create the vertex object array that'll store the mesh render info */
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  /* Create a buffer object to store vertex data in */
  glGenBuffers(1, &vertex_data_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_data_buffer);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3),
               points.data(), GL_STATIC_DRAW);

  /* Set the attrib pointer to point to our point info */
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

  /* Unbind the data buffer and the vertex array */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Map::addVertex(glm::vec3& point) {
  printf("Point added: %f %f %f\n", point.x, point.y, point.z);
  points.push_back(glm::vec3(point));
}
