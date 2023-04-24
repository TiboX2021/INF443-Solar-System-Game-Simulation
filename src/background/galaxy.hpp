#pragma once

#include "environment.hpp"

/**
 * Galaxy environment. Sphere that we are inside of.
 It must move along with the player !
*/
class Galaxy
{

public:
    // Constructor
    Galaxy();
    Galaxy(double radius);

    // Draw function
    void initialize();
    void draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true);

    // Setters
    void setPosition(cgp::vec3 position);

private:
    double radius;
    cgp::vec3 position;

    // CGP elements
    cgp::mesh galaxy_mesh;
    cgp::mesh_drawable galaxy_mesh_drawable;
};