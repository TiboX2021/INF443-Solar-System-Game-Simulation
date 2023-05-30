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
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(direction, -PLAYER_MAX_ROLL_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    directionTop = rotation_transform_obj.matrix() * directionTop;
}

void PlayerObject::rollRight()
{
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(direction, +PLAYER_MAX_ROLL_SPEED * Timer::getSimulStep());

    // Rotate both vectors
    directionTop = rotation_transform_obj.matrix() * directionTop;
}

void PlayerObject::updatePlayerCamera(custom_camera_model &camera_model) const
{
    // TODO : when upating camera, see the top of the ship ? Just a rotation ? This will be better with animations
    // TODO: assifier ça en une fonction qui donnerait dirrect ce dont on a besoin (+ pour faire le retard de rotation par rapport au vaisseau)
    // Ajouter une petite rotation
    auto rotation_axis = cgp::cross(direction, directionTop);
    auto additional_rotation = cgp::rotation_transform::from_axis_angle(rotation_axis, -0.3f);

    camera_model.direction = direction;
    camera_model.top = directionTop;
    camera_model.camera_position = Object::scaleDownDistanceForDisplay(position) - additional_rotation * direction * 10.0f; // Add camera offset to be able to see the spaceship
}

void PlayerObject::updatePlayerShip(Navion &ship) const
{
    // TODO : also set orientation
    // ship.set_direction(direction);
    // ship.set_position(Object::scaleDownDistanceForDisplay(position));
    ship.hierarchie["Centre"].transform_local.translation = Object::scaleDownDistanceForDisplay(position);
    ship.hierarchie["Centre"].transform_local.rotation = orientation();
    ship.hierarchie.update_local_to_global_coordinates();
}

cgp::rotation_transform PlayerObject::orientation() const
{
    // Rotate to the base direction
    auto match_directions = cgp::rotation_transform::from_vector_transform(PLAYER_BASE_DIRECTION, direction);

    // Once the directions match, rotate AROUND the directions in order to make the top directions match
    auto match_tops = cgp::rotation_transform::from_vector_transform(match_directions * PLAYER_BASE_TOP, directionTop);

    return match_tops * match_directions;
}