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
 * @brief Class that abstracts camera manipulation and math.
 */

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    this->pos = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    CalcCameraVectors();
}

void Camera::ProcessKeyboard(Movement direction, float delta_time,
	                     bool sprint) {
    /* Determine how quickly we should move the camera */
    float velocity = delta_time * MOVEMENT_SPEED;
    if (sprint)
	velocity *= SPRINT_MODIFIER;

    /* Move position of camera based on what direction is desired */
    if (direction == FORWARD)
        pos += front * velocity;
    if (direction == BACKWARD)
        pos -= front * velocity;
    if (direction == LEFT)
        pos -= right * velocity;
    if (direction == RIGHT)
        pos += right * velocity;
}

void Camera::ProcessMouseMovement(float x_offset, float y_offset,
		                  bool constrain_pitch) {
    /* Lessen the movement speed by a sensitivity coefficient */
    x_offset *= MOUSE_SENSITIVITY;
    y_offset *= MOUSE_SENSITIVITY;

    /* Offset pitch and yaw by the calculated movement offsets */
    yaw += x_offset;
    pitch += y_offset;

    /* Lock pitch to a fixed min/max */
    if (constrain_pitch) {
        if (pitch > CAMERA_PITCH_CONSTRAINT)
            pitch = CAMERA_PITCH_CONSTRAINT;
        if (pitch < -CAMERA_PITCH_CONSTRAINT)
            pitch = -CAMERA_PITCH_CONSTRAINT;
    }

    /* Recalculate vectors */
    CalcCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    //TODO: explain math
    return glm::lookAt(pos, pos + front, up);
}

void Camera::CalcCameraVectors() {
    /* Recalculate front, right, and up vectors */
    //TODO: explain math
    front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
