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
 * @brief Class that abstracts texture operations.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <exception>
#include <glad/glad.h>


class TextureException : public std::exception {
public:
    TextureException(std::string msg) {
        this->msg = msg;
    }

    const char* what() const throw() {
        return this->msg.c_str();
    }

private:
    std::string msg;
};


class Texture {
public:
    Texture(const std::string& texture_file, GLenum active_texture=GL_TEXTURE0);

    void Use();

private:
    GLuint texture;
    GLenum active_texture;
    int width;
    int height;
    int channels;
};


#endif // TEXTURE_H
