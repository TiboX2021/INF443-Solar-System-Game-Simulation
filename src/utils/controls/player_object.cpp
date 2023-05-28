#include "player_object.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "utils/camera/custom_camera_model.hpp"
#include "utils/physics/object.hpp"
#include <cmath>
#include <iostream>

void PlayerObject::step()
{
    // TODO : rotation via rotation speed (currently directly during key events)

    // Translation
    position += velocity * Timer::getSimulStep();
}

// Translation functions

// Accelerate with animation
void PlayerObject::moveForward()
{
    speed += PLAYER_MAX_TRANSLATION_ACCELERATION * Timer::getSimulStep();

    // Cap speed
    if (speed > PLAYER_MAX_TRANSLATION_SPEED)
        speed = PLAYER_MAX_TRANSLATION_SPEED;

    // Apply speed to object model
    velocity = direction * speed;
}

// Brake with animation
void PlayerObject::brake()
{
    speed -= PLAYER_MAX_TRANSLATION_ACCELERATION * Timer::getSimulStep();

    // Cap speed
    if (speed < 0)
        speed = 0;

    // Apply speed to object model
    velocity = direction * speed;
}

// TODO : movement functions
// TODO : rotate the direction axis (with a rotation speed)
// Problème : up / down, etc : il faut savoir où est le dessus ?
void PlayerObject::moveUp()
{
    // TODO : rotation plus difficile si on va vite
    // Rotate towards the "up" vector (which is perpendicular to the direction vector)
    cgp::vec3 rotation_axis = cgp::cross(direction, directionTop);
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(rotation_axis, PLAYER_MAX_ROTATION_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    direction = rotation_transform_obj.matrix() * direction;
    directionTop = rotation_transform_obj.matrix() * directionTop;
}

void PlayerObject::moveDown()
{
    cgp::vec3 rotation_axis = cgp::cross(direction, directionTop);
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(rotation_axis, -PLAYER_MAX_ROTATION_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    direction = rotation_transform_obj.matrix() * direction;
    directionTop = rotation_transform_obj.matrix() * directionTop;
}

void PlayerObject::moveLeft()
{
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(directionTop, +PLAYER_MAX_ROTATION_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    direction = rotation_transform_obj.matrix() * direction;
}

void PlayerObject::moveRight()
{
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(directionTop, -PLAYER_MAX_ROTATION_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    direction = rotation_transform_obj.matrix() * direction;
}

void PlayerObject::rollLeft()
{
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(direction, -PLAYER_MAX_ROTATION_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    directionTop = rotation_transform_obj.matrix() * directionTop;
}

void PlayerObject::rollRight()
{
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(direction, +PLAYER_MAX_ROTATION_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    directionTop = rotation_transform_obj.matrix() * directionTop;
}

void PlayerObject::updatePlayerCamera(custom_camera_model &camera_model) const
{
    camera_model.direction = direction;
    camera_model.top = directionTop;
    camera_model.camera_position = Object::scaleDownDistanceForDisplay(position);
}