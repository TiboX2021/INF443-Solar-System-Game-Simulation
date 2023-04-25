#pragma once

#include "cgp/geometry/vec/vec3/vec3.hpp"

/*
PHYSICS_SCALE :
Distance between Sun & Pluto : 5.9e+12 m
Max OpenGL distance of rendering : 10 000 UA
Mapping 1e+4 UA <=> 1e+13 m
The physics scale is then set to 1e-9

This way, realistic physiscs distances and constants can be directly used in the simulation

=> the masses and forces and positions of the physics engine are real
=> the positions of the meshes are scaled down
=> the radiuses of the meshes too ?
*/

// Pour les collisions, faire une gestion maligne pour limiter le temps de calcul

// Physics constants
constexpr double GRAVITAIONAL_CONSTANT = 6.67408e-11;
constexpr double PHYSICS_SCALE = 1e-9;
constexpr double DISPLAY_SCALE = 1000; // Display larger models

/**
 * Physical Object with position and rotation abstract class
 */
class Object
{
public:
    Object(double mass, cgp::vec3 position, cgp::vec3 rotation_axis = {0, 0, 1}, bool should_translate = true, bool should_rotate = true);

    // TODO : function to compute force, and to update with dt. speed, acceleration, etc?
    void update(double dt);

    void resetForces();
    void computeGravitationnalForce(Object *other);
    // virtual void setPosition(cgp::vec3 position); // Faire une autre fonction qui set la position physique réelle ?
    virtual void updateModels() = 0; // TODO : function to update the models based on the physical constants

    // Getters
    cgp::vec3 getPhysicsPosition() const;
    double getPhysicsRotationAngle() const;

    // Setters
    void setShouldTranslate(bool should_translate);
    void setShouldRotate(bool should_rotate);
    void setPhysicsPosition(cgp::vec3 position);

    // STATIC MEMBERS
    // Scale a value to the physics scale (distance and mass)
    static cgp::vec3
    scaleDownDistanceForDisplay(cgp::vec3 distance)
    {
        return distance * PHYSICS_SCALE;
    }
    static double scaleRadiusForDisplay(double value) { return value * DISPLAY_SCALE * PHYSICS_SCALE; }
    static void setTimeScale(double timescale) // Asset time scale is greater than 1
    {
        if (timescale >= 1.0)
            time_scale = timescale;
        else
            time_scale = 1.0;
    }
    static double getTimeScale() { return time_scale; }

private:
    // Translation
    double mass;
    cgp::vec3 physics_position;
    cgp::vec3 velocity;
    cgp::vec3 acceleration;
    cgp::vec3 forces;

    // TODO Rotation
    double rotation_speed;
    double rotation_angle;
    cgp::vec3 rotation_axis;

    // Immobile object or not
    bool should_translate;
    bool should_rotate;

    static double time_scale;
};