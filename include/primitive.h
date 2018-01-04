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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include <exception>
#include <initializer_list>
#include <glad/glad.h>
#include "shader.h"

class PrimitiveException : public std::exception {
public:
    PrimitiveException(std::string msg) {
        this->msg = msg;
    }

    const char* what() const throw() {
        return this->msg.c_str();
    }

 private:
    std::string msg;
};

/**
 * Class that handles the creation and rendering of an OpenGL primitive.
 *
 * This class can handle two kinds of primitives; regular and colored.  Regular
 * primitives require the fragment shader to handle color setting independently,
 * whereas colored primitives will supply the shader with colors.  This class
 * expects shaders it uses to accept the following attributes:
 *   - 'position': Position to draw the vertex at.
 *   - 'color': Color of the vertex.  Only used if colors are supplied.
 *
 *  Vertices should be supplied in the following formats:
 *  ```
 *  // REGULAR PRIMITIVE
 *  primitive = new Primitive(shader, {
 *  //   X     Y     Z
 *      0.0f, 0.5f, 0.0f,
 *      0.5f, 0.0f, 0.0f,
 *     -0.5f, 0.0f, 0.0f,
 *  })
 *
 *  // COLORED PRIMITIVE
 *  primitive = new Primitive(shader, {
 *  //   X     Y     Z     R     G     B
 *      0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
 *      0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
 *     -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
 *  })
 *  ```
 */
class Primitive {
public:
    Primitive(std::initializer_list<GLfloat> vertices,
              Shader* shader, bool colored=false);

    void Render(bool use_shader=true);

private:
    GLuint vbo;  /** Vertex buffer object that contains this primitive's verts */
    GLuint vao;  /** Vertex array object that contains info used to render */
    Shader* shader;  /** Shader to render with */

    void Bind();
    void Unbind();
};

#endif // PRIMITIVE_H
