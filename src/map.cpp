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
#include "bsp_parser.h"

MapFace::MapFace(Shader* shader, std::vector<uint16_t>& indices) {
    this->shader = shader;
    face_color = glm::vec3(float(rand()) / RAND_MAX,
                           float(rand()) / RAND_MAX,
                           float(rand()) / RAND_MAX);

    /* Create the vertex object array that'll store the mesh render info */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* Create a buffer object to store vertex data in */
    glGenBuffers(1, &element_bo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_bo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)(indices.size() * sizeof(uint16_t)),
                 indices.data(), GL_STATIC_DRAW);
    index_amt = indices.size();

    /* Set the attrib pointer to point to our point info */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    /* Unbind the data buffer and the vertex array */
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MapFace::render() {
    shader->SetVec3("face_color", face_color);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLE_STRIP, index_amt, GL_UNSIGNED_SHORT, 0);
}

Map::Map() {
  shader = nullptr;
  vertex_bo = -1;
}

void Map::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
  shader->Use();
  shader->SetMat4("projection", projection);
  shader->SetMat4("view", view);
  shader->SetMat4("model", model);

  for (MapFace face : faces) {
      face.render();
  }
}

void Map::FromBSP(BSPParser* parser) {
  shader = new Shader("./assets/shaders/level.glsl");

  /* Create a buffer object to store vertex data in */
  glGenBuffers(1, &vertex_bo);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_bo);
  glBufferData(GL_ARRAY_BUFFER, parser->vertices.size() * sizeof(glm::vec3),
               parser->vertices.data(), GL_STATIC_DRAW);

  /* Create faces */
  for (bsp_face_t face : parser->map_faces) {
      /* Extract each point in the edge */
      std::vector<uint16_t> points;
      for (int edge_index=face.first_edge; edge_index < face.first_edge + face.num_edges; edge_index++) {
          uint16_t edge1;
          uint16_t edge2;

          bsp_surfedge_t surfedge = parser->map_surfedges[edge_index];
          bsp_edge_t edge = parser->map_edges[abs(surfedge)];
          if (surfedge < 0) {
              edge1 = edge.v[0];
              edge2 = edge.v[1];
          } else {
              edge1 = edge.v[1];
              edge2 = edge.v[0];
          }

          /* Add the point indices to the array */
          points.push_back(edge1);
          points.push_back(edge2);
      }

      faces.push_back(MapFace(shader, points));
  }

  /* Unbind the vertex buffer */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
