#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
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

// Display constants
constexpr double PHYSICS_SCALE = 1e-10; // Reduce the scale enough so that Opengl does not freak out
constexpr double DISPLAY_SCALE = 1500;  // Display larger models
constexpr double ORBIT_FACTOR = 20;     // Make orbits faster in comparison to planets rotating on themselves, for display purposes

/**
 * Physical Object with position and rotation abstract class
 */
class Object
{
public:
    Object(double mass, cgp::vec3 position, cgp::vec3 rotation_axis = {0, 0, 1}, bool should_translate = true, bool should_rotate = true);

    void update(double dt);

    void resetForces();
    void computeGravitationnalForce(Object *other);
    virtual void updateModels(){}; // Abstract function to update the models based on the physical constants

    // Getters
    cgp::vec3 getPhysicsPosition() const;
    cgp::vec3 getPhysicsVelocity() const;
    cgp::rotation_transform getPhysicsRotation() const;
    double getPhysicsRotationAngle() const;
    bool getShouldTranslate() const;
    bool getShouldRotate() const;
    double getMass() const;

    // Setters
    void setShouldTranslate(bool should_translate);
    void setShouldRotate(bool should_rotate);
    void setPhysicsPosition(cgp::vec3 position);
    void setInitialVelocity(cgp::vec3 velocity);
    void setInitialRotationSpeed(double rotation_speed);
    void setRotationAxis(cgp::vec3 rotation_axis);

    // STATIC MEMBERS
    // Scale a value to the physics scale (distance and mass)
    static cgp::vec3
    scaleDownDistanceForDisplay(cgp::vec3 distance)
    {
        return distance * PHYSICS_SCALE;
    }
    static double scaleRadiusForDisplay(double value) { return value * DISPLAY_SCALE * PHYSICS_SCALE; }
    static double computeOrbitalSpeed(double M, double r);
    static cgp::vec3 computeOrbitalSpeedForPosition(double M, cgp::vec3 position, cgp::vec3 rotation_axis = {0, 0, 1});

private:
    // Translation
    double mass;
    cgp::vec3 physics_position;
    cgp::vec3 velocity;
    cgp::vec3 acceleration;
    cgp::vec3 forces;

    // Rotation
    double rotation_speed;
    double rotation_angle;
    cgp::vec3 rotation_axis;

    // Immobile object or not
    bool should_translate;
    bool should_rotate;
};