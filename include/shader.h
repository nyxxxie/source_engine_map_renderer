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
 * @brief Class that abstracts shader operations.
 *
 * This file contains the entry point, argument parsing stuff, and other logic
 * related to starting the program.
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <exception>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderException : public std::exception {
public:
    ShaderException(std::string msg) {
        this->msg = msg;
    }

    const char* what() const throw() {
        return this->msg.c_str();
    }

private:
    std::string msg;
};

class Shader {
public:
    Shader(const std::string& glsl_file);

    void Use();
    GLuint id();
    GLint GetUniformLocation(const std::string& name, bool except=true);
    void SetInt(const std::string& name, GLint value);
    void SetFloat(const std::string& name, GLfloat value);
    void SetVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z);
    void SetVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void SetMat4(const std::string& name, const glm::mat4& mat, GLboolean transpose=GL_FALSE);

private:
    GLuint program_id;

    GLuint CreateShaderFromString(std::string shader_src, GLenum shadertype,
                                  char* infolog, const size_t infolog_size);
};

#endif // SHADER_H
