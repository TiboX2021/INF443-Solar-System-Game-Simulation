#pragma once

#include "environment.hpp"
#include "utils/display/base_drawable.hpp"

/**
 * Abstract base drawable class
 */
class Drawable : private BaseDrawable
{
public:
    // Initialize member meshs
    virtual void initialize(){};

    // Draw function
    virtual void draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) = 0;

    // Setters
    virtual void setPosition(cgp::vec3 position) = 0;

    // Getters
    virtual cgp::vec3 getPosition() const = 0;
};