#include "custom_camera_model.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include <iostream>

cgp::vec3 custom_camera_model::position() const
{
    return camera_position;
}

cgp::rotation_transform custom_camera_model::orientation() const
{
    // Rotate to the base direction
    auto match_directions = cgp::rotation_transform::from_vector_transform(BASE_DIRECTION, direction);

    // Once the directions match, rotate AROUND the directions in order to make the top directions match
    auto match_tops = cgp::rotation_transform::from_vector_transform(match_directions * BASE_TOP, top);

    return match_tops * match_directions;
}