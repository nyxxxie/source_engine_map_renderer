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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.h"


Shader::Shader(const std::string& vertex_shader_path,
               const std::string& fragment_shader_path) {
    GLuint vertex_shader, fragment_shader;
    std::ifstream vertex_src, fragment_src;
    char infolog[512];
    GLint success;

    /* Open vertex shader file */
    std::ifstream vertex_shader_file(vertex_shader_path);
    if (!vertex_shader_file.good()) {
        throw ShaderException("Failed to find vertex shader file at "
                              + vertex_shader_path);
    }
    std::stringstream vertex_shader_stream;
    vertex_shader_stream << vertex_shader_file.rdbuf();
    std::string vertex_shader_src = vertex_shader_stream.str();
    vertex_shader_file.close();

    /* Compile vertex shader */
    std::cout << "---------------------------" << std::endl;
    std::cout << vertex_shader_src << std::endl;
    std::cout << "---------------------------" << std::endl;
    vertex_shader = CreateShaderFromString(vertex_shader_src.c_str(),
		    			   GL_VERTEX_SHADER,
					   infolog, sizeof(infolog));
    if (!vertex_shader) {
        throw ShaderException("Failed to compile vertex shader: "
                              + std::string(infolog));
    }

    /* Open fragment shader file */
    std::ifstream fragment_shader_file(fragment_shader_path);
    if (!fragment_shader_file.good()) {
        throw ShaderException("Failed to find fragment shader file at "
                              + fragment_shader_path);
    }
    std::stringstream fragment_shader_stream;
    fragment_shader_stream << fragment_shader_file.rdbuf();
    std::string fragment_shader_src = fragment_shader_stream.str();
    fragment_shader_file.close();

    /* Fragment vertex shader */
    std::cout << "---------------------------" << std::endl;
    std::cout << fragment_shader_src << std::endl;
    std::cout << "---------------------------" << std::endl;
    fragment_shader = CreateShaderFromString(fragment_shader_src.c_str(),
		    			     GL_FRAGMENT_SHADER,
                                             infolog, sizeof(infolog));
    if (!fragment_shader) {
        throw ShaderException("Failed to compile fragment shader: "
                              + std::string(infolog));
    }

    /* Create shader program */
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, vertex_shader);
    glAttachShader(this->program_id, fragment_shader);
    glLinkProgram(this->program_id);
    glGetProgramiv(this->program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->program_id, sizeof(infolog), NULL, infolog);
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
        throw ShaderException("Failed to link shader program: "
                              + std::string(infolog));
    }

    /* We don't need these shaders after we've linked them, clean up shaders */
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
}

/**
 *
 */
void Shader::Use() {
    glUseProgram(this->id());
}
/**
 *
 */
GLuint Shader::id() {
    return this->program_id;
}

/**
 *
 */
void Shader::SetUniform(const std::string& name, GLint value) {
    int loc = glGetUniformLocation(this->id(), name.c_str());
    if (loc < 0) {
        throw ShaderException("Failed to find shader uniform \"" + name +"\"");
    }

    glUniform1i(loc, value);
}

/**
 *
 */
void Shader::SetUniform(const std::string& name, GLfloat value) {
    int loc = glGetUniformLocation(this->id(), name.c_str());
    if (loc < 0) {
        throw ShaderException("Failed to find shader uniform \"" + name +"\"");
    }

    glUniform1f(loc, value);
}

/**
 *
 */
GLuint Shader::CreateShaderFromString(std::string shader_src, GLenum shadertype,
				      char* infolog, const size_t infolog_size) {
    GLuint shader_id;
    GLint success;

    /* Create vertex shader */
    const char* src = shader_src.c_str();
    shader_id = glCreateShader(shadertype);
    glShaderSource(shader_id, 1, &src, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, infolog_size, NULL, infolog);
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}
