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

#include <stdint.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "bsp_file.h"
#include "map.h"


/**
 *
 */
class BSPParserException : public std::exception {
 public:
  BSPParserException(std::string msg) {
    this->msg = msg;
  }

  const char* what() const throw() {
    return this->msg.c_str();
  }

 private:
  std::string msg;
};


/**
 *
 */
class BSPParser {
  public:
    BSPParser(std::string path);

    Map* genMap();

  private:
    std::vector<glm::vec3> vertices;
    std::vector<bsp_edge_t> edges;
    std::vector<bsp_surfedge_t> surfedges;
    std::vector<bsp_face_t> faces;

    void processHeader(uint8_t* data, size_t data_len);
    void processLump(uint8_t* data, size_t data_len, uint32_t lump_type, bsp_lump_t* lump);
    void processVertexLump(uint8_t* data, size_t data_len, bsp_lump_t* lump);
    void processEdgeLump(uint8_t* data, size_t data_len, bsp_lump_t* lump);
    void processSurfedgeLump(uint8_t* data, size_t data_len, bsp_lump_t* lump);
    void processFaceLump(uint8_t* data, size_t data_len, bsp_lump_t* lump);
};
