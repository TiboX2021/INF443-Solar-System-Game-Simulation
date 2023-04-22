#include "environment.hpp"
#include "utils/noise/perlin.hpp"
#include <string>

#pragma once

/**
 * Planète avec son apparence visuelle et ses propriétés physiques
 */
class Planet
{
public:
    Planet();
    Planet(float radius, vec3 position, std::string texture_path = "assets/planets/mars.jpg");

    // Draw function
    void initialize();
    void draw(environment_structure const &environment, bool show_wireframe = true) const;

    // Setters
    void set_radius(float radius);
    void set_position(vec3 position);

private:
    // Perlin noise properties
    perlin_noise_parameters parameters;

    // Planet properties
    float radius;
    vec3 position;
    std::string texture_path;

    // CGP elements
    cgp::mesh planet_mesh;
    cgp::mesh_drawable planet_mesh_drawable;
};