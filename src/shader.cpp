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
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.h"


Shader::Shader(const std::string& glsl_path) {
    GLuint vertex_shader, fragment_shader;
    std::stringstream glsl_stream;
    std::ifstream glsl_file(glsl_path);
    char infolog[512];
    GLint success;

    /* Open vertex shader file */
    if (!glsl_file.good()) {
        throw ShaderException("Failed to find glsl file at " + glsl_path);
    }
    glsl_stream << glsl_file.rdbuf();
    std::string glsl_src = glsl_stream.str();
    glsl_file.close();

    /* Compile vertex shader */
    vertex_shader = CreateShaderFromString(glsl_src.c_str(),
                                           GL_VERTEX_SHADER,
                                           infolog, sizeof(infolog));
    if (!vertex_shader) {
        throw ShaderException("Failed to compile vertex shader: "
                              + std::string(infolog));
    }

    /* Fragment vertex shader */
    fragment_shader = CreateShaderFromString(glsl_src.c_str(),
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
GLint Shader::GetUniformLocation(const std::string& name, bool except) {
    GLint loc = glGetUniformLocation(this->id(), name.c_str());
    if (loc < 0 && except) {
        throw ShaderException("Failed to find shader uniform \"" + name +"\"");
    }

    return loc;
}

/**
 *
 */
void Shader::SetInt(const std::string& name, GLint value) {
    glUniform1i(GetUniformLocation(name), value);
}

/**
 *
 */
void Shader::SetFloat(const std::string& name, GLfloat value) {
    glUniform1f(GetUniformLocation(name), value);
}

/**
 *
 */
void Shader::SetVec3(const std::string& name, const glm::vec3& vec) {
    SetVec3(name, vec.x, vec.y, vec.z);
}

/**
 *
 */
void Shader::SetVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) {
    glUniform3f(GetUniformLocation(name), x, y, z);
}

/**
 *
 */
void Shader::SetVec4(const std::string& name, const glm::vec4& vec) {
    SetVec4(name, vec.x, vec.y, vec.z, vec.w);
}

/**
 *
 */
void Shader::SetVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}

/**
 *
 */
void Shader::SetMat4(const std::string& name, const glm::mat4& mat, GLboolean transpose) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

/**
 *
 */
GLuint Shader::CreateShaderFromString(std::string shader_src, GLenum shadertype,
                      char* infolog, const size_t infolog_size) {
    GLuint shader_id;
    GLint success;

    /* Prepend appropriate preprocessor definition to compile the proper shader
       in the file */
    switch(shadertype) {
    case GL_VERTEX_SHADER:
        shader_src = std::string("#define VERTEX_SHADER\n") + shader_src;
        break;
    case GL_FRAGMENT_SHADER:
        shader_src = std::string("#define FRAGMENT_SHADER\n") + shader_src;
        break;
    }

    /* Prepend shader version */
    // TODO: keep this in the glsl files, find a good way to insert the above
    //       preprocessor definitions after the version declaration line
    shader_src = std::string("#version 330 core\n") + shader_src;

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
