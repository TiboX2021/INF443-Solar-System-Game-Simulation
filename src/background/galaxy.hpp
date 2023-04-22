#pragma once

#include "environment.hpp"

// TODO : create a reversed sphere with the same texture as the skybox, and the same position as the camera position
// TODO : il ne faut pas qu'elle subisse l'illumination de la lumière, mais qu'elle soit toujours éclairée
// par la même luminosité !

/**
 * Galaxy environment. Sphere that we are inside of.
 It must move along with the player !
*/
class Galaxy
{

public:
    // Constructor
    Galaxy();
    Galaxy(double radius, cgp::vec3 position);

    // Draw function
    void initialize();
    void draw(environment_structure const &environment, bool show_wireframe = true) const;

private:
    double radius;
    cgp::vec3 position;

    // CGP elements
    cgp::mesh galaxy_mesh;
    cgp::mesh_drawable galaxy_mesh_drawable;
};