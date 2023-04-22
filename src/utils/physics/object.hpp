#pragma once

#include "cgp/geometry/vec/vec3/vec3.hpp"

const double GRAVITAIONAL_CONSTANT = 6.67408e-11;

class Object
{
public:
    Object(double mass, cgp::vec3 position);

    // TODO : function to compute force, and to update with dt. speed, acceleration, etc?
    void update(double dt); // TODO : how to do things with other objects?

    void reset_forces();
    void compute_force(Object *other);

private:
    double mass;
    cgp::vec3 position;
    cgp::vec3 velocity;
    cgp::vec3 acceleration;
    cgp::vec3 forces;
};