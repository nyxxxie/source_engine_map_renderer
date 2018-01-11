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

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#define MOVEMENT_SPEED 3.0f
#define MOUSE_SENSITIVITY 0.1f
#define CAMERA_YAW -90.0f
#define CAMERA_PITCH 0.0f
#define CAMERA_PITCH_CONSTRAINT 89.0f
#define SPRINT_MODIFIER 10.0f


class Camera {
public:
    enum Movement {
        FORWARD=0,
	BACKWARD,
	LEFT,
	RIGHT
    };

    glm::vec3 pos;

    Camera(glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f),
	   float yaw=CAMERA_YAW, float pitch=CAMERA_PITCH);

    void ProcessKeyboard(Movement direction, float delta_time, bool sprint=false);
    void ProcessMouseMovement(float x_offset, float y_offset, bool constrain_pitch=true);
    glm::mat4 GetViewMatrix();

private:
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    float yaw;
    float pitch;

    void CalcCameraVectors();
};


#endif // CAMERA_H
