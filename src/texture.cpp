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

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "graphics/stb_image.h"


Texture::Texture(const std::string& texture_file, GLenum active_texture,
		 GLenum format, bool flip) {
    this->active_texture = active_texture;

    /* Create and bind opengl texture object */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Set texture wrapping parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /* Set texture filtering parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Flip texture if requested */
    if (flip) {
        stbi_set_flip_vertically_on_load(true);
    }

    /* Load texture from image file */
    unsigned char* data = stbi_load(texture_file.c_str(), &width, &height,
		                    &channels, 0);
    if (!data) {
        throw TextureException("Failed to load texture.");
    }

    /* Load texture into GPU */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    /* Generate mipmaps */
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Clean up data */
    stbi_image_free(data);
}

void Texture::Use() {
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}

