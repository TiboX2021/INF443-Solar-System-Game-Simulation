#include "player_object.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "utils/camera/custom_camera_model.hpp"
#include "utils/controls/gui_params.hpp"
#include "utils/display/display_constants.hpp"
#include "utils/opengl/shield_ubo.hpp"
#include "utils/physics/object.hpp"
#include "utils/tools/tools.hpp"
#include <cmath>
#include <iostream>

// Define global player position
ReadWriteLock<PlayerCollisionData> global_player_collision_data;

// Global player collision animation buffer
AsteroidCollisionAnimationBuffer global_player_collision_animation_buffer(SHIELD_COLLISION_BUFFER_MAX_SIZE, SHIELD_COLLISION_ANIMATION_TIME);

void PlayerObject::step(const std::vector<Object *> &objects_with_hitbox)
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

    // Push back the position if it collides with an object
    check_clip_and_push_back(objects_with_hitbox, position, 1 / PHYSICS_SCALE);

    // Renormalize direction vectors (else they will drift)
    direction = cgp::normalize(direction);
    directionTop = cgp::normalize(directionTop);

    global_player_collision_data.write({position, velocity, PLAYER_SHIELD_RADIUS / PHYSICS_SCALE});
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

void PlayerObject::updatePlayerCamera(custom_camera_model &camera_model, const std::vector<Object *> &camera_clip_objects) const
{
    camera_model.direction = camera_direction;
    camera_model.top = camera_direction_top;

    cgp::vec3 physics_camera_position = position - camera_direction * global_gui_params.camera_distance_atomic / PHYSICS_SCALE;

    // Clip the camera (use the physics radius or the display radius ? Here the physics)
    for (auto &object : camera_clip_objects)
    {
        // Check if the camera is inside the object
        if (object->isInside(physics_camera_position))
        {
            physics_camera_position = circle_intersect_point(object->getPhysicsPosition(), object->getPhysicsRadius() + 0.01 / (PHYSICS_SCALE * DISPLAY_SCALE), position, cgp::normalize(physics_camera_position - position));
        }
    }

    camera_model.camera_position = Object::scaleDownDistanceForDisplay(physics_camera_position);
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

cgp::vec3 PlayerObject::get_position() const
{
    return position;
}

cgp::vec3 PlayerObject::get_direction() const
{
    return direction;
}