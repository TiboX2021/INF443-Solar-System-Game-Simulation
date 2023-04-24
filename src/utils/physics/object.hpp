#pragma once

#include "cgp/geometry/vec/vec3/vec3.hpp"

/*
PHYSICS_SCALE :
Distance between Sun & Pluto : 5.9e+12 m
Max OpenGL distance of rendering : 10 000 UA
Mapping 1e+4 UA <=> 1e+13 m
The physics scale is then set to 1e-9

This way, realistic physiscs distances and constants can be directly used in the simulation
*/

// TODO : prendre en compte la rotation des objets ! Il faudra prendre en compte les collisions aussi
// Pour les collisions, faire une gestion maligne pour limiter le temps de calcul

// Physics constants
constexpr double GRAVITAIONAL_CONSTANT = 6.67408e-11;
constexpr double PHYSICS_SCALE = 1e-9;

class Object
{
public:
    Object(double mass, cgp::vec3 position);

    // TODO : function to compute force, and to update with dt. speed, acceleration, etc?
    void update(double dt);

    void reset_forces();
    void compute_force(Object *other);
    virtual void setPosition(cgp::vec3 position);

    // STATIC MEMBERS

    // Scale a value to the physics scale (distance and mass)
    static double scale(double value) { return value * PHYSICS_SCALE; }
    static void setTimeScale(double timescale) // Asset time scale is greater than 1
    {
        if (timescale >= 1.0)
            time_scale = timescale;
        else
            time_scale = 1.0;
    }
    static double getTimeScale() { return time_scale; }

private:
    double mass;
    cgp::vec3 position;
    cgp::vec3 velocity;
    cgp::vec3 acceleration;
    cgp::vec3 forces;

    static double time_scale;
};