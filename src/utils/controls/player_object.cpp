#include "player_object.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "utils/camera/custom_camera_model.hpp"
#include "utils/physics/object.hpp"
#include "utils/tools/tools.hpp"
#include <cmath>
#include <iostream>

void PlayerObject::step()
{
    // Roll speed
    auto roll_rotation_transform_obj = cgp::rotation_transform::from_axis_angle(direction, roll_speed.value * Timer::getSimulStep());

    // Only rotate the top vector
    directionTop = roll_rotation_transform_obj.matrix() * directionTop;

    // Vertical rotation speed
    cgp::vec3 rotation_axis = cgp::cross(direction, directionTop);
    auto vertical_rotation_transform_obj = cgp::rotation_transform::from_axis_angle(rotation_axis, vertical_rotation_speed.value * Timer::getSimulStep());

    // Rotate both vectors
    direction = vertical_rotation_transform_obj.matrix() * direction;
    directionTop = vertical_rotation_transform_obj.matrix() * directionTop;

    // Horizontal rotation speed
    auto rotation_transform_obj = cgp::rotation_transform::from_axis_angle(directionTop, horizontal_rotation_speed.value * Timer::getSimulStep());

    // Only rotate the direction vector
    direction = rotation_transform_obj.matrix() * direction;

    // Update camera values with ones from buffer
    camera_direction = vector_mix(camera_direction_buffer.getNextOne(), direction, DELAY_RATIO);
    camera_direction_top = vector_mix(camera_direction_top_buffer.getNextOne(), directionTop, DELAY_RATIO);
    // Force perpendicularization to avoid bugs
    camera_direction_top = perpendicular_projection(camera_direction_top, camera_direction);

    // Set camera diection buffer values
    camera_direction_buffer.add(camera_direction);
    camera_direction_top_buffer.add(camera_direction_top);

    // Translation
    position += velocity * Timer::getSimulStep();
}

// Translation functions

// Accelerate with animation
void PlayerObject::moveForward()
{
    speed.one_step_up();

    // Apply speed to object model
    velocity = direction * speed.value;
}

// Brake with animation
void PlayerObject::brake()
{
    speed.one_step_down();

    // Apply speed to object model
    velocity = direction * speed.value;
}

void PlayerObject::moveUp()
{
    vertical_rotation_speed.one_step_up();
}

void PlayerObject::moveDown()
{
    vertical_rotation_speed.one_step_down();
}

void PlayerObject::moveLeft()
{
    horizontal_rotation_speed.one_step_up();
}

void PlayerObject::moveRight()
{
    horizontal_rotation_speed.one_step_down();
}

void PlayerObject::rollLeft()
{
    roll_speed.one_step_down();
}

void PlayerObject::rollRight()
{
    roll_speed.one_step_up();
}

void PlayerObject::updatePlayerCamera(custom_camera_model &camera_model) const
{
    camera_model.direction = camera_direction;
    camera_model.top = camera_direction_top;
    camera_model.camera_position = Object::scaleDownDistanceForDisplay(position) - camera_direction * 10.0f;
}

void PlayerObject::updatePlayerShip(Navion &ship) const
{
    ship.set_position(Object::scaleDownDistanceForDisplay(position));
    ship.set_orientation(orientation());
    ship.update_hierachy();
}

cgp::rotation_transform PlayerObject::orientation() const
{
    // Rotate to the base direction
    auto match_directions = cgp::rotation_transform::from_vector_transform(PLAYER_BASE_DIRECTION, direction);

    // Once the directions match, rotate AROUND the directions in order to make the top directions match
    auto match_tops = cgp::rotation_transform::from_vector_transform(match_directions * PLAYER_BASE_TOP, directionTop);

    return match_tops * match_directions;
}

void PlayerObject::decelerateRoll()
{
    roll_speed.one_step_decelerate();
}

void PlayerObject::decelerateHorizontalRotation()
{
    horizontal_rotation_speed.one_step_decelerate();
}

void PlayerObject::decelerateVerticalRotation()
{
    vertical_rotation_speed.one_step_decelerate();
}