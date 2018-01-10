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
 * @brief Tools for animating transitions between two values over time
 *
 * This file provides a set of tools for transitioning a vector from one value
 * to another over a period of time.  This is useful for everything from camera
 * movement to moving objects in a scene.  The goal of tools in this file is to
 * expose a number of formulas that allow 
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <math.h>  // TODO: included for NAN, find something more standard?
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class AnimationException : public std::exception {
public:
    AnimationException(std::string msg) {
        this->msg = msg;
    }

    const char* what() const throw() {
        return this->msg.c_str();
    }

private:
    std::string msg;
};


/**
 * Base class for implementing animations
 */
class Animation {
public:
    Animation(glm::vec4 start_value, double start_time);

    virtual void reset(double start_time = NAN);
    virtual glm::mat4 CalcTransform(const glm::mat4&, double current_time, const glm::vec3& current_value);
};


/**
 * @brief Animation between one or more vectors.
 *
 * This animation implements a keyframe-based system where the user defines
 * several destination endpoints and transitions are eased over a specified time
 * by an easing function to smoothly transit between each of those points.
 */
class Transition : public Animation {
public:
    Transition(glm::vec4 start_value, double start_time);

    void reset(double start_time = NAN);
    void AddKeyframe(double duration, glm::mat4 dest_value, int transition_type);
    glm::mat4 CalcTransform(const glm::mat4& transform, double current_time, const glm::vec3& current_value);
}


#endif // ANIMATION_H
