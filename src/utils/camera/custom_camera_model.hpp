#pragma once

#include "cgp/graphics/camera/camera_model/camera_generic_base/camera_generic_base.hpp"

// These variables are not set by me, but by the cgp engine, that considers this as the default
// (Determined by sending a blank rotation as the camera orientation)
const cgp::vec3 BASE_DIRECTION = {0, 0, -1};
const cgp::vec3 BASE_TOP = {0, 1, 0};

// Camera controller that uses vectors in order to determine position and orientation
struct custom_camera_model : cgp::camera_generic_base
{
    cgp::vec3 direction = {1, 0, 0};
    cgp::vec3 top = {0, 0, 1}; // Perpendicular to the direction, points to the top
    cgp::vec3 camera_position = {0, 0, 0};

    // Main orientation functions
    virtual cgp::vec3 position() const override;
    virtual cgp::rotation_transform orientation() const override;
};