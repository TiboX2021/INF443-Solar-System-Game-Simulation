#pragma once

#include "environment.hpp"
#include "utils/noise/perlin.hpp"
#include <string>

/**
 * Planète avec son apparence visuelle et ses propriétés physiques
 */
class Planet
{
public:
    Planet();
    Planet(double radius, vec3 position, std::string texture_path = "assets/planets/mars.jpg");

    // Draw function
    void initialize();
    void draw(environment_structure const &environment, bool show_wireframe = true) const;

    // Setters
    void set_radius(double radius);
    void set_position(vec3 position);

    // Utility physics functions
    double get_height_at(vec3 position) const;

private:
    // Perlin noise properties
    perlin_noise_parameters parameters;

    // Planet properties
    double radius;
    vec3 position;
    std::string texture_path;

    // CGP elements
    cgp::mesh planet_mesh;
    cgp::mesh_drawable planet_mesh_drawable;
};