#pragma once

#include "environment.hpp"
#include "utils/display/base_drawable.hpp"

/**
 * Abstract base drawable class
 */
class Drawable : public virtual BaseDrawable
{
public:
    // Initialize member meshs
    virtual void initialize(){};

    // Draw function
    virtual void draw(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true) = 0;

    // Setters
    virtual void setPosition(cgp::vec3 position) = 0;

    // Getters
    virtual cgp::vec3 getPosition() const = 0;
};